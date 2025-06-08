import { LeafMethod } from "./callable"
import IObj from "./object"

export default class LeafClass implements IObj {
    private constructor(
        private _fields: Set<string>,
        private _methods: Map<string, LeafMethod>
    ) {}

    static createInstance(
        fields: Set<string>,
        methods: Map<string, LeafMethod>
    ) {
        return new LeafClass(fields, methods)
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
    private constructor(private _fields: Map<string, IObj>) {}

    static createInstance(fields: Map<string, IObj>) {
        return new LeafInstance(fields)
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
