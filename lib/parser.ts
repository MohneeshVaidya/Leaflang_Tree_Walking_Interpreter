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
import IStmt, {
    BlockStmt,
    BreakStmt,
    ConstStmt,
    ExprStmt,
    ForStmt,
    ForWrapperStmt,
    IfStmt,
    InfiniteForStmt,
    LetForStmt,
    LetStmt,
    PrintlnStmt,
    PrintStmt,
    WhileForStmt,
} from "./stmt"
import Token from "./token"
import tokenType, { TokenType } from "./tokenType"

export default class Parser {
    private constructor(
        private _tokens: Token[],
        private _stmts: IStmt[],
        private _current: number
    ) {}

    static createInstance() {
        return new Parser([], [], 0)
    }

    getStmts(tokens: Token[]) {
        this._tokens = tokens
        while (!this.isAtEnd()) {
            this._stmts.push(this.statement())
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
        throw new Error(msg)
    }

    private peekPrev() {
        return this._tokens[this._current - 1]
    }

    private matchPrev(...tokenTypes: TokenType[]) {
        const type = this.peekPrev().type()
        return tokenTypes.includes(type)
    }

    // statements
    private statement(): IStmt {
        if (this.match(tokenType.PRINT)) return this.printStmt()
        if (this.match(tokenType.PRINTLN)) return this.printlnStmt()
        if (this.match(tokenType.LET)) return this.letStmt()
        if (this.match(tokenType.CONST)) return this.constStmt()
        if (this.match(tokenType.LEFT_BRACE)) return this.blockStmt()
        if (this.match(tokenType.IF)) return this.ifStmt()
        if (this.match(tokenType.FOR)) return this.forStmt()
        if (this.match(tokenType.BREAK)) return this.breakStmt()
        return this.expressionStmt()
    }

    private printStmt(): IStmt {
        const line = this.peekPrev().line()
        const expr = this.expression()
        this.expect(tokenType.SEMICOLON, line, "a statement must end with ';'")
        return PrintStmt.createInstance(expr)
    }

    private printlnStmt(): IStmt {
        const line = this.peekPrev().line()
        const expr = this.expression()
        this.expect(tokenType.SEMICOLON, line, "a statement must end with ';'")
        return PrintlnStmt.createInstance(expr)
    }

    private letStmt(): IStmt {
        const line = this.peekPrev().line()
        const identifier = this.expect(
            tokenType.IDENTIFIER,
            line,
            "a name is expected after let keyword (name should not be a keyword provided by language)"
        )
        if (this.match(tokenType.SEMICOLON)) {
            return LetStmt.createInstance(identifier, NilExpr.createInstance())
        }
        this.expect(
            tokenType.EQUAL,
            line,
            `'=' is required after '${identifier.lexeme()}'`
        )
        const expr = this.expression()

        this.expect(tokenType.SEMICOLON, line, "a statement must end with ';'")

        return LetStmt.createInstance(identifier, expr)
    }

    private constStmt(): IStmt {
        const line = this.peekPrev().line()
        const identifier = this.expect(
            tokenType.IDENTIFIER,
            line,
            "a name is expected after const keyword (name should not be a keyword provided by language)"
        )
        this.expect(
            tokenType.EQUAL,
            line,
            `a constant declaration requires an initializer`
        )
        const expr = this.expression()

        this.expect(tokenType.SEMICOLON, line, "a statement must end with ';'")

        return ConstStmt.createInstance(identifier, expr)
    }

    private blockStmt(): IStmt {
        const stmts: IStmt[] = []
        while (!this.match(tokenType.RIGHT_BRACE)) {
            stmts.push(this.statement())
        }
        return BlockStmt.createInstance(stmts)
    }

    private ifStmt(): IStmt {
        const line = this.peekPrev().line()

        const stmtTable: [IExpr, BlockStmt][] = []

        const condition = this.expression()
        this.expect(
            tokenType.LEFT_BRACE,
            line,
            "'{' is expected after condition expression of 'if'"
        )
        stmtTable.push([condition, this.blockStmt() as BlockStmt])

        while (this.match(tokenType.ELSEIF)) {
            const line = this.peekPrev().line()
            const condition = this.expression()
            this.expect(
                tokenType.LEFT_BRACE,
                line,
                "'{' is expected after condition expression of 'elseif'"
            )
            stmtTable.push([condition, this.blockStmt() as BlockStmt])
        }

        if (this.match(tokenType.ELSE)) {
            const line = this.peekPrev().line()
            this.expect(
                tokenType.LEFT_BRACE,
                line,
                "'{' is expected after 'else' keyword"
            )
            stmtTable.push([
                LiteralExpr.createInstance(
                    Token.createInstance(tokenType.TRUE, "true", 0)
                ),
                this.blockStmt() as BlockStmt,
            ])
        }

        return IfStmt.createInstance(stmtTable)
    }

    private forStmt(): IStmt {
        const getSemiColonNum = (line: number) => {
            let num = 0
            let idx = this._current
            while (
                idx < this._tokens.length - 1 &&
                this._tokens[idx].type() !== tokenType.LEFT_BRACE &&
                num < 3
            ) {
                if (this._tokens[idx].type() === tokenType.SEMICOLON) ++num
                ++idx
            }
            if (idx >= this._tokens.length || num >= 3) {
                LeafError.getInstance().throwRunTimeError(
                    line,
                    "'for' statement is incomplete"
                )
            }
            return num
        }

        const infiniteFor = () => {
            const stmts = this.blockStmt() as BlockStmt
            return InfiniteForStmt.createInstance(stmts)
        }

        const whileFor = (line: number) => {
            const condition = this.expression()
            this.expect(
                tokenType.LEFT_BRACE,
                line,
                "'{' is expected after condition expression of 'for'"
            )
            const stmts = this.blockStmt() as BlockStmt
            return WhileForStmt.createInstance(condition, stmts)
        }

        const stdFor = (line: number) => {
            const init = this.expressionStmt() as ExprStmt
            const condition = this.expressionStmt() as ExprStmt
            const step = this.expression()
            this.expect(
                tokenType.LEFT_BRACE,
                line,
                "'{' is expected after step expression of 'for' statement"
            )
            const stmts = this.blockStmt() as BlockStmt
            return ForStmt.createInstance(
                init.expr(),
                condition.expr(),
                step,
                stmts
            )
        }

        const letFor = (line: number) => {
            const init = this.letStmt() as LetStmt
            const condition = this.expressionStmt() as ExprStmt
            const step = this.expression()
            this.expect(
                tokenType.LEFT_BRACE,
                line,
                "'{' is expected after step expression of 'for' statement"
            )
            const stmts = this.blockStmt() as BlockStmt
            return LetForStmt.createInstance(
                init,
                condition.expr(),
                step,
                stmts
            )
        }

        const line = this.peekPrev().line()
        if (this.match(tokenType.LEFT_BRACE)) {
            return ForWrapperStmt.createInstance(infiniteFor())
        }

        const num = getSemiColonNum(line)

        if (num === 0) {
            return ForWrapperStmt.createInstance(whileFor(line))
        } else if (this.match(tokenType.LET)) {
            return ForWrapperStmt.createInstance(letFor(line))
        }
        return ForWrapperStmt.createInstance(stdFor(line))
    }

    private breakStmt(): IStmt {
        const stmt = BreakStmt.createInstance(this.peekPrev())
        this.expect(
            tokenType.SEMICOLON,
            stmt.keyword().line(),
            "a statement must end with ';'"
        )
        return stmt
    }

    private expressionStmt(): IStmt {
        const stmt = ExprStmt.createInstance(this.expression())
        this.expect(tokenType.SEMICOLON, 0, "a statement must end with ';'")
        return stmt
    }

    // expressions
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
