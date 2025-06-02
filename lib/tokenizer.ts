import LeafError from "./error"
import utils from "./utils"
import Token from "./token"
import tokenType, { type TokenType } from "./tokenType"

export default class Tokenizer {
    private constructor(
        private source: string = "",
        private tokens: Token[] = [],
        private start: number = 0,
        private end: number = 0,
        private line: number = 1
    ) {}

    static createInstance() {
        return new Tokenizer()
    }

    getTokens(source: string) {
        this.source = source

        while (true) {
            const char = this.getChar()
            if (char === "") {
                this.addToken(tokenType.EOF)
                break
            }
            this.prepare(char)
        }
        return this.tokens
    }

    // private helpers
    private isAtEnd() {
        return this.end >= this.source.length
    }

    private advance() {
        if (!this.isAtEnd()) {
            ++this.end
        }
    }

    private getChar() {
        if (this.isAtEnd()) {
            return ""
        }
        this.advance()
        return this.source[this.end - 1]
    }

    private peekChar() {
        if (this.isAtEnd()) {
            return ""
        }
        return this.source[this.end]
    }

    private peekNextChar() {
        if (this.end + 1 >= this.source.length) {
            return ""
        }
        return this.source[this.end + 1]
    }

    private match(...targets: string[]) {
        const char = this.peekChar()
        for (let target of targets) {
            if (char === target) {
                this.advance()
                return true
            }
        }
        return false
    }

    private skipws() {
        while (true) {
            const char = this.peekChar()
            if (!utils.isSpace(char)) {
                break
            }
            this.advance()
        }
        this.start = this.end
    }

    private skipComment() {
        while (true) {
            const char = this.peekChar()
            if (char === "\n") {
                ++this.line
                break
            } else if (char === "") {
                break
            }
            this.advance()
        }
    }

    private string() {
        this.start = this.end

        while (true) {
            const char = this.peekChar()
            if (char === '"') {
                break
            }
            if (char === "" || char === "\n") {
                LeafError.getInstance().addLexicalError(
                    this.line,
                    "a string must end with '\"';"
                )
                return
            }
            this.advance()
        }
        this.addToken(tokenType.STRING)
        this.advance()
        this.start = this.end
    }

    private fmtString() {
        this.start = this.end

        const getFmtStrTokens = () => {
            const i = this.end
            let val = 1
            while (val > 0) {
                switch (this.peekChar()) {
                    case "{":
                        ++val
                        break
                    case "}":
                        --val
                        break
                    case "\n":
                        this.line++
                    case "":
                        LeafError.getInstance().addLexicalError(
                            this.line,
                            "a string must end with '\"';"
                        )
                        throw 1
                }
                this.advance()
            }
            return Tokenizer.createInstance().getTokens(
                this.source.slice(i, this.end - 1)
            )
        }

        const tokens: Token[][] = []

        while (true) {
            const char = this.peekChar()
            if (char === '"') {
                break
            }
            if (char === "" || char === "\n") {
                LeafError.getInstance().addLexicalError(
                    this.line,
                    "a string must end with '\"';"
                )
                return
            }
            if (char === "$" && this.peekNextChar() === "{") {
                this.end += 2
                try {
                    tokens.push(getFmtStrTokens())
                } catch (_) {
                    return
                }
            } else {
                this.advance()
            }
        }
        this.addFmtStrToken(tokens)
        this.advance()
        this.start = this.end
    }

    private number() {
        while (true) {
            const char = this.peekChar()
            if (!(utils.isNumeric(char) || char === ".")) {
                break
            }
            this.advance()
        }
        this.addToken(tokenType.NUMBER)
    }

    private identifier() {
        while (true) {
            const char = this.peekChar()
            if (!utils.isAlphaNumeric(char)) {
                break
            }
            this.advance()
        }
        this.addIdentifierToken()
    }

    private prepare(char: string) {
        if (utils.isSpace(char)) return this.skipws()
        if (char === '"') return this.string()
        if (char === "f" && this.match('"')) return this.fmtString()
        if (utils.isNumeric(char)) return this.number()
        if (utils.isAlphabetic(char)) return this.identifier()

        switch (char) {
            case "+":
                this.addToken(tokenType.PLUS)
                break
            case "-":
                this.addToken(tokenType.MINUS)
                break
            case "/":
                if (this.match("/")) {
                    this.skipComment()
                } else {
                    this.addToken(tokenType.SLASH)
                }
                break
            case "*":
                if (this.match("*")) {
                    this.addToken(tokenType.STAR_STAR)
                } else {
                    this.addToken(tokenType.STAR)
                }
                break
            case "%":
                this.addToken(tokenType.PERCENT)
                break
            case "=":
                if (this.match("=")) {
                    this.addToken(tokenType.EQUAL_EQUAL)
                } else {
                    this.addToken(tokenType.EQUAL)
                }
                break
            case "!":
                if (this.match("=")) {
                    this.addToken(tokenType.BANG_EQUAL)
                } else {
                    this.addToken(tokenType.BANG)
                }
                break
            case "<":
                if (this.match("=")) {
                    this.addToken(tokenType.LESSER_EQUAL)
                } else {
                    this.addToken(tokenType.LESSER)
                }
                break
            case ">":
                if (this.match("=")) {
                    this.addToken(tokenType.GREATER_EQUAL)
                } else {
                    this.addToken(tokenType.GREATER)
                }
                break
            case "(":
                this.addToken(tokenType.LEFT_PAREN)
                break
            case ")":
                this.addToken(tokenType.RIGHT_PAREN)
                break
            case "{":
                this.addToken(tokenType.LEFT_BRACE)
                break
            case "}":
                this.addToken(tokenType.RIGHT_BRACE)
                break
            case ",":
                this.addToken(tokenType.COMMA)
                break
            case ";":
                this.addToken(tokenType.SEMICOLON)
                break
            case ":":
                this.addToken(tokenType.COLON)
                break
            case ".":
                this.addToken(tokenType.DOT)
                break
            case "\n":
                this.addToken(tokenType.NEWLINE)
                this.line++
                break
            case "?":
                this.addToken(tokenType.QUESTION)
                break
        }
    }

    private addToken(type: TokenType) {
        this.tokens.push(
            Token.createInstance(
                type,
                this.source.slice(this.start, this.end),
                this.line
            )
        )
        this.start = this.end
    }

    private addFmtStrToken(tokens: Token[][]) {
        this.tokens.push(
            Token.createInstance(
                tokenType.FMT_STRING,
                this.source.slice(this.start, this.end),
                this.line,
                tokens
            )
        )
        this.start = this.end
    }

    private addIdentifierToken() {
        const lexeme = this.source.slice(this.start, this.end)
        switch (lexeme) {
            case "print":
                this.addToken(tokenType.PRINT)
                break
            case "println":
                this.addToken(tokenType.PRINTLN)
                break
            case "let":
                this.addToken(tokenType.LET)
                break
            case "const":
                this.addToken(tokenType.CONST)
                break
            case "if":
                this.addToken(tokenType.IF)
                break
            case "elseif":
                this.addToken(tokenType.ELSEIF)
                break
            case "else":
                this.addToken(tokenType.ELSE)
                break
            case "for":
                this.addToken(tokenType.FOR)
                break
            case "break":
                this.addToken(tokenType.BREAK)
                break
            case "continue":
                this.addToken(tokenType.CONTINUE)
                break
            case "func":
                this.addToken(tokenType.FUNC)
                break
            case "return":
                this.addToken(tokenType.RETURN)
                break
            case "class":
                this.addToken(tokenType.CLASS)
                break
            case "this":
                this.addToken(tokenType.THIS)
                break
            case "super":
                this.addToken(tokenType.SUPER)
                break
            case "extends":
                this.addToken(tokenType.EXTENDS)
                break
            case "__make":
                this.addToken(tokenType.MAKE)
                break
            case "and":
                this.addToken(tokenType.AND)
                break
            case "or":
                this.addToken(tokenType.OR)
                break
            case "xor":
                this.addToken(tokenType.XOR)
                break
            case "true":
                this.addToken(tokenType.TRUE)
                break
            case "false":
                this.addToken(tokenType.FALSE)
                break
            case "nil":
                this.addToken(tokenType.NIL)
                break
            default:
                this.addToken(tokenType.IDENTIFIER)
        }
    }
}
