import { type TokenType } from "./tokenType"

export default class Token {
    private constructor(
        private type: TokenType,
        private lexeme: string,
        private line: number,
        private fmtStrTokens?: Token[][]
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

    getType() {
        return this.type
    }

    getLexeme() {
        return this.lexeme
    }

    getLine() {
        return this.line
    }

    getTokens() {
        return this.fmtStrTokens
    }

    toString(): string {
        return `Token{
    type   = ${this.type}
    lexeme = ${this.lexeme}
    line   = ${this.line}
}`
    }
}
