export default interface IObj {
    value(): any
}

export class ObjString implements IObj {
    private constructor(private _value: string) {}

    static createInstance(value: string) {
        return new ObjString(value)
    }

    value() {
        return this._value
    }
}

export class ObjNumber implements IObj {
    private constructor(private _value: number) {}

    static createInstance(value: number) {
        return new ObjNumber(value)
    }

    value() {
        return this._value
    }
}

export class ObjBool implements IObj {
    private constructor(private _value: boolean) {}

    static createInstance(value: boolean) {
        return new ObjBool(value)
    }

    value() {
        return this._value
    }
}

export class ObjNil implements IObj {
    private constructor() {}

    static createInstance() {
        return new ObjNil()
    }

    value(): any {
        return null
    }
}
