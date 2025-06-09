import LeafError from "./error"
import utils from "./utils"
import Token from "./token"
import tokenType, { type TokenType } from "./tokenType"

export default class Tokenizer {
    private constructor(
        private _source: string = "",
        private _tokens: Token[] = [],
        private _start: number = 0,
        private _end: number = 0,
        private _line: number = 1
    ) {}

    static createInstance() {
        return new Tokenizer()
    }

    getTokens(source: string) {
        this._source = source

        while (true) {
            const char = this.get()
            if (char === "") {
                this.addToken(tokenType.EOF)
                break
            }
            this.prepare(char)
        }
        return this._tokens
    }

    // private helpers
    private isAtEnd() {
        return this._end >= this._source.length
    }

    private advance() {
        if (!this.isAtEnd()) {
            ++this._end
        }
    }

    private get() {
        if (this.isAtEnd()) {
            return ""
        }
        this.advance()
        return this._source[this._end - 1]
    }

    private peek() {
        if (this.isAtEnd()) {
            return ""
        }
        return this._source[this._end]
    }

    private peekNext() {
        if (this._end + 1 >= this._source.length) {
            return ""
        }
        return this._source[this._end + 1]
    }

    private peekPrev() {
        return this._source[this._end - 1]
    }

    private match(...targets: string[]) {
        const char = this.peek()
        for (let target of targets) {
            if (char === target) {
                this.advance()
                return true
            }
        }
        return false
    }

    private skipws() {
        if (this.peekPrev() === "\n") {
            ++this._line
        }
        while (true) {
            const char = this.peek()
            if (!utils.isSpace(char)) {
                break
            }
            if (char === "\n") {
                ++this._line
            }
            this.advance()
        }
        this._start = this._end
    }

    private skipComment() {
        while (true) {
            const char = this.peek()
            if (char === "\n") {
                ++this._line
                break
            } else if (char === "") {
                break
            }
            this.advance()
        }
    }

    private string() {
        this._start = this._end

        while (true) {
            const char = this.peek()
            if (char === '"') {
                break
            }
            if (char === "" || char === "\n") {
                LeafError.getInstance().addLexicalError(
                    this._line,
                    "a string must end with '\"';"
                )
                return
            }
            this.advance()
        }
        this.addToken(tokenType.STRING)
        this.advance()
        this._start = this._end
    }

    private fmtString() {
        this._start = this._end

        const getFmtStrTokens = () => {
            const i = this._end
            let val = 1
            while (val > 0) {
                switch (this.peek()) {
                    case "{":
                        ++val
                        break
                    case "}":
                        --val
                        break
                    case "\n":
                        this._line++
                    case "":
                        LeafError.getInstance().addLexicalError(
                            this._line,
                            "a string must end with '\"';"
                        )
                        throw 1
                }
                this.advance()
            }
            return Tokenizer.createInstance().getTokens(
                this._source.slice(i, this._end - 1)
            )
        }

        const tokens: Token[][] = []

        while (true) {
            const char = this.peek()
            if (char === '"') {
                break
            }
            if (char === "" || char === "\n") {
                LeafError.getInstance().addLexicalError(
                    this._line,
                    "a string must end with '\"';"
                )
                return
            }
            if (char === "$" && this.peekNext() === "{") {
                this._end += 2
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
        this._start = this._end
    }

    private number() {
        while (true) {
            const char = this.peek()
            if (!(utils.isNumeric(char) || char === ".")) {
                break
            }
            this.advance()
        }
        this.addToken(tokenType.NUMBER)
    }

    private identifier() {
        while (true) {
            const char = this.peek()
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
                } else if (this.match(">")) {
                    this.addToken(tokenType.EQUAL_GREATER)
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
            case "?":
                this.addToken(tokenType.QUESTION)
                break
        }
    }

    private addToken(type: TokenType) {
        this._tokens.push(
            Token.createInstance(
                type,
                this._source.slice(this._start, this._end),
                this._line
            )
        )
        this._start = this._end
    }

    private addFmtStrToken(tokens: Token[][]) {
        this._tokens.push(
            Token.createInstance(
                tokenType.FMT_STRING,
                this._source.slice(this._start, this._end),
                this._line,
                tokens
            )
        )
        this._start = this._end
    }

    private addIdentifierToken() {
        const lexeme = this._source.slice(this._start, this._end)
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
            case "constructor":
                this.addToken(tokenType.CONSTRUCTOR)
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
