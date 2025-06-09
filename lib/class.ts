import { LeafMethod } from "./callable"
import IObj from "./object"
import Token from "./token"

export default class LeafClass implements IObj {
    private constructor(
        private _name: Token,
        private _fields: Set<string>,
        private _methods: Map<string, LeafMethod>
    ) {}

    static createInstance(
        name: Token,
        fields: Set<string>,
        methods: Map<string, LeafMethod>
    ) {
        return new LeafClass(name, fields, methods)
    }

    name() {
        return this._name
    }

    fields() {
        return this._fields
    }

    methods() {
        return this._methods
    }

    value() {
        return "class { }"
    }
}

export class LeafInstance implements IObj {
    private constructor(
        private _className: Token,
        private _fields: Map<string, IObj>
    ) {}

    static createInstance(className: Token, fields: Map<string, IObj>) {
        return new LeafInstance(className, fields)
    }

    className() {
        return this._className
    }

    fields() {
        return this._fields
    }

    value() {
        return `{
${Array.from(this._fields.entries())
    .map(([key, value]) => `    ${key}: ${value.value()}`)
    .join("\n")}        
}`
    }
}
