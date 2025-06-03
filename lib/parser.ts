import type IExpr from "./expr"
import Token from "./token"

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
        return this._current >= this._tokens.length
    }

    private expression(): IExpr {
        throw 1
    }
}
