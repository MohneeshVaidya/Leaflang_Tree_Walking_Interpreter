export default class LeafError {
    private _errors: string[]
    private _hasErrors: boolean

    private constructor() {
        this._errors = []
        this._hasErrors = false
    }

    private static _instance: LeafError

    static getInstance() {
        if (typeof this._instance === "undefined") {
            this._instance = new LeafError()
        }
        return this._instance
    }

    errors() {
        return this._errors
    }

    hasErrors() {
        return this._hasErrors
    }

    addLexicalError(line: number, msg: string) {
        this.addError("lexical_error", `[near line ${line}] - ${msg}`)
    }

    makeParsingError(line: number, msg: string) {
        return new Error(`parsing_error: [near line ${line}] - ${msg}`)
    }

    makeRuntimeError(line: number, msg: string) {
        return new Error(`runtime_error: [near line ${line}] - ${msg}`)
    }

    private addError(type: string, msg: string) {
        this._hasErrors = true
        this._errors.push(`${type}: ${msg}`)
    }
}
