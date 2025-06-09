import { type TokenType } from "./tokenType"

export default class Token {
    private constructor(
        private _type: TokenType,
        private _lexeme: string,
        private _line: number,
        private _fmtStrTokens?: Token[][]
    ) {}

    private static instance: Token

    static createInstance(
        type: TokenType,
        lexeme: string,
        line: number,
        fmtStrTokens?: Token[][]
    ) {
        return new Token(type, lexeme, line, fmtStrTokens)
    }

    type() {
        return this._type
    }

    lexeme() {
        return this._lexeme
    }

    line() {
        return this._line
    }

    tokens() {
        return this._fmtStrTokens
    }

    toString(): string {
        return `Token{
    type   = ${this._type}
    lexeme = ${this._lexeme}
    line   = ${this._line}
}`
    }
}
