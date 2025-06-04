import LeafError from "./error"
import type IExpr from "./expr"
import {
    AssignExpr,
    BinaryExpr,
    ExponentExpr,
    GroupingExpr,
    IdentifierExpr,
    LiteralExpr,
    NilExpr,
    TernaryExpr,
    UnaryExpr,
} from "./expr"
import Token from "./token"
import tokenType, { TokenType } from "./tokenType"

export default class Parser {
    private constructor(
        private _tokens: Token[],
        private _stmts: IExpr[],
        private _current: number
    ) {}

    static createInstance() {
        return new Parser([], [], 0)
    }

    getStmts(tokens: Token[]) {
        this._tokens = tokens
        while (!this.isAtEnd()) {
            this._stmts.push(this.expression())
        }
        return this._stmts
    }

    private isAtEnd() {
        return this._current >= this._tokens.length - 1
    }

    private advance() {
        if (!this.isAtEnd()) {
            ++this._current
        }
    }

    private get() {
        if (this.isAtEnd()) {
            return this._tokens.at(-1) as Token
        }
        ++this._current
        return this._tokens[this._current - 1]
    }

    private peek() {
        if (this.isAtEnd()) {
            return this._tokens.at(-1) as Token
        }
        return this._tokens[this._current]
    }

    private match(...tokenTypes: TokenType[]) {
        const type = this.peek().type()
        if (tokenTypes.includes(type)) {
            this.advance()
            return true
        }
        return false
    }

    private expect(type: TokenType, line: number, msg: string) {
        if (this.get().type() === type) {
            return this.peekPrev()
        }
        LeafError.getInstance().addParsingError(line, msg)
        throw new Error("synchronize")
    }

    private peekPrev() {
        return this._tokens[this._current - 1]
    }

    private matchPrev(...tokenTypes: TokenType[]) {
        const type = this.peekPrev().type()
        return tokenTypes.includes(type)
    }

    private expression(): IExpr {
        return this.assignExpr()
    }

    private assignExpr(): IExpr {
        const left = this.ternaryExpr()
        if (this.match(tokenType.EQUAL)) {
            const oper = this.peekPrev()
            const right = this.assignExpr()
            return AssignExpr.createInstance(oper, left, right)
        }
        return left
    }

    private ternaryExpr(): IExpr {
        const condition = this.orExpr()
        if (this.match(tokenType.QUESTION)) {
            const operQuest = this.peekPrev()
            const first = this.ternaryExpr()
            const operColon = this.expect(
                tokenType.COLON,
                operQuest.line(),
                "':' is expected;"
            )
            const second = this.ternaryExpr()
            return TernaryExpr.createInstance(
                condition,
                operQuest,
                first,
                operColon,
                second
            )
        }
        return condition
    }

    private orExpr(): IExpr {
        let left = this.andExpr()
        while (this.match(tokenType.OR)) {
            const oper = this.peekPrev()
            const right = this.andExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private andExpr(): IExpr {
        let left = this.equalityExpr()
        while (this.match(tokenType.AND)) {
            const oper = this.peekPrev()
            const right = this.equalityExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private equalityExpr(): IExpr {
        let left = this.comparisionExpr()
        while (this.match(tokenType.EQUAL_EQUAL, tokenType.BANG_EQUAL)) {
            const oper = this.peekPrev()
            const right = this.comparisionExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private comparisionExpr(): IExpr {
        let left = this.termExpr()
        while (
            this.match(
                tokenType.LESSER_EQUAL,
                tokenType.LESSER,
                tokenType.GREATER,
                tokenType.GREATER_EQUAL
            )
        ) {
            const oper = this.peekPrev()
            const right = this.termExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private termExpr(): IExpr {
        let left = this.factorExpr()
        while (this.match(tokenType.PLUS, tokenType.MINUS)) {
            const oper = this.peekPrev()
            const right = this.factorExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private factorExpr(): IExpr {
        let left = this.exponentExpr()
        while (this.match(tokenType.STAR, tokenType.SLASH, tokenType.PERCENT)) {
            const oper = this.peekPrev()
            const right = this.exponentExpr()
            left = BinaryExpr.createInstance(oper, left, right)
        }
        return left
    }

    private exponentExpr(): IExpr {
        let left = this.unaryExpr()
        if (this.match(tokenType.STAR_STAR)) {
            const oper = this.peekPrev()
            const right = this.exponentExpr()
            left = ExponentExpr.createInstance(oper, left, right)
        }
        return left
    }

    private unaryExpr(): IExpr {
        if (this.match(tokenType.BANG, tokenType.MINUS)) {
            const oper = this.peekPrev()
            const expr = this.primaryExpr()
            return UnaryExpr.createInstance(oper, expr)
        }
        return this.primaryExpr()
    }

    private primaryExpr(): IExpr {
        const token = this.get()

        if (this.matchPrev(tokenType.LEFT_PAREN)) {
            return this.groupingExpr()
        }

        if (
            this.matchPrev(
                tokenType.TRUE,
                tokenType.FALSE,
                tokenType.STRING,
                tokenType.NUMBER
            )
        ) {
            return LiteralExpr.createInstance(token)
        }

        if (this.matchPrev(tokenType.IDENTIFIER)) {
            return IdentifierExpr.createInstance(token)
        }

        return NilExpr.createInstance()
    }

    private groupingExpr(): IExpr {
        const leftParen = this.peekPrev()
        const expr = this.expression()
        this.expect(
            tokenType.RIGHT_PAREN,
            leftParen.line(),
            "every '(' should be accompanied by a matching ')';"
        )
        return GroupingExpr.createInstance(leftParen, expr)
    }
}
