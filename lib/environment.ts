import LeafError from "./error"
import IObj from "./object"
import Token from "./token"

export type OptionalEnvironment = Environment | null

export default class Environment {
    private _table: Map<string, IObj>
    private _parent: OptionalEnvironment

    private constructor(parent: OptionalEnvironment) {
        this._table = new Map()
        this._parent = parent
    }

    static createInstance(parent: OptionalEnvironment = null) {
        return new Environment(parent)
    }

    private hasIdentifier(identifier: Token) {
        return (
            this._table.has("const_" + identifier.lexeme()) ||
            this._table.has("let_" + identifier.lexeme())
        )
    }

    parent() {
        return this._parent
    }

    setParent(parent: Environment) {
        this._parent = parent
    }

    insertLet(identifier: Token, value: IObj) {
        if (this.hasIdentifier(identifier)) {
            LeafError.getInstance().throwRunTimeError(
                identifier.line(),
                `name '${identifier.lexeme()}' re-declared in same scope`
            )
        }
        this._table.set("let_" + identifier.lexeme(), value)
    }

    insertConst(identifier: Token, value: IObj) {
        if (this.hasIdentifier(identifier)) {
            LeafError.getInstance().throwRunTimeError(
                identifier.line(),
                `name '${identifier.lexeme()}' re-declared in same scope`
            )
        }
        this._table.set("const_" + identifier.lexeme(), value)
    }

    set(identifier: Token, value: IObj) {
        if (this._table.has("let_" + identifier.lexeme())) {
            return this._table.set("let_" + identifier.lexeme(), value)
        }

        if (this._table.has("const_" + identifier.lexeme())) {
            LeafError.getInstance().throwRunTimeError(
                identifier.line(),
                `const '${identifier.lexeme()}' can not be re-assigned a value`
            )
        }

        if (this._parent === null) {
            LeafError.getInstance().throwRunTimeError(
                identifier.line(),
                `name '${identifier.lexeme()}' is used before declaration`
            )
        } else {
            this._parent.set(identifier, value)
        }
    }

    get(identifier: Token): IObj {
        if (this.hasIdentifier(identifier)) {
            return (this._table.get("const_" + identifier.lexeme()) ??
                this._table.get("let_" + identifier.lexeme())) as IObj
        }

        if (this._parent === null) {
            LeafError.getInstance().throwRunTimeError(
                identifier.line(),
                `name '${identifier.lexeme()}' is used before declaration`
            )
            throw ""
        } else {
            return this._parent.get(identifier)
        }
    }
}
