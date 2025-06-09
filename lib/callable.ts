import LeafClass, { LeafInstance } from "./class"
import Environment from "./environment"
import LeafError from "./error"
import IExpr from "./expr"
import Interpreter from "./interpreter"
import IObj, { ObjNil } from "./object"
import { BlockStmt } from "./stmt"
import Token from "./token"
import utils from "./utils"

export type MethodType = "method" | "constructor"

const constructorOrMethod: MethodType[] = ["constructor", "method"]

export default abstract class Callable {
    protected constructor(
        protected _parameters: Token[],
        protected _stmts: BlockStmt,
        protected _closure: Environment
    ) {}

    parameters() {
        return this._parameters
    }

    stmts() {
        return this._stmts
    }

    closure() {
        return this._closure
    }

    abstract call(args: IExpr[], interpreter: Interpreter, expr: IExpr): IObj

    // helper methods
    protected execSharedCallCode(
        args: IExpr[],
        interpreter: Interpreter,
        expr: IExpr
    ) {
        if (args.length !== this.parameters().length) {
            throw LeafError.getInstance().makeRuntimeError(
                interpreter.getCallLine(expr),
                "number of arguments is not equal to number of parameters"
            )
        }

        const argsEvaluated = args.map((arg) => interpreter.evaluate(arg))

        const prevEnvironment = interpreter.environment()
        interpreter.setEnvironment(Environment.createInstance(this._closure))

        args.forEach((_, idx) => {
            interpreter
                .environment()
                .insertLet(this._parameters[idx], argsEvaluated[idx])
        })

        return prevEnvironment
    }
}

export class LeafFunction extends Callable implements IObj {
    private constructor(
        parameters: Token[],
        stmts: BlockStmt,
        environment: Environment
    ) {
        super(parameters, stmts, environment)
    }

    static createInstance(
        parameters: Token[],
        stmts: BlockStmt,
        environment: Environment
    ) {
        return new LeafFunction(parameters, stmts, environment)
    }

    call(args: IExpr[], interpreter: Interpreter, expr: IExpr): IObj {
        const prevEnvironment = this.execSharedCallCode(args, interpreter, expr)

        try {
            interpreter.execute(this._stmts.stmts())

            interpreter.setEnvironment(prevEnvironment)

            return ObjNil.createInstance()
        } catch (err) {
            interpreter.setEnvironment(prevEnvironment)

            if (err instanceof ReturnValue) {
                return err.value()
            }
            throw err
        }
    }

    value() {
        return `func (${this._parameters
            .map((parameter) => parameter.lexeme())
            .join(", ")}) { }`
    }
}

export class LeafMethod extends Callable implements IObj {
    private _instance: LeafInstance
    private _type: MethodType
    private _className: LeafClass

    private constructor(
        parameters: Token[],
        stmts: BlockStmt,
        environment: Environment
    ) {
        super(parameters, stmts, environment)
    }

    static createInstance(
        parameters: Token[],
        stmts: BlockStmt,
        environment: Environment
    ) {
        return new LeafMethod(parameters, stmts, environment)
    }

    instance() {
        return this._instance
    }

    setInstance(instance: LeafInstance) {
        this._instance = instance
    }

    type() {
        return this._type
    }

    setType(type: MethodType) {
        this._type = type
    }

    className() {
        return this._className
    }

    setClassName(className: LeafClass) {
        this._className = className
    }

    call(args: IExpr[], interpreter: Interpreter, expr: IExpr): IObj {
        const prevEnvironment = this.execSharedCallCode(args, interpreter, expr)

        if (this._type === "constructor") {
            const fields = utils.makeInstanceFieldsTable(
                this._className.fields()
            )
            this._instance = LeafInstance.createInstance(
                this._className.name(),
                fields
            )
        }

        const prevInstance = interpreter.instance()
        interpreter.setInstance(this._instance)

        try {
            interpreter.execute(this._stmts.stmts())

            interpreter.setEnvironment(prevEnvironment)
            interpreter.setInstance(prevInstance)

            if (this._type === "constructor") {
                return this._instance
            }
            return ObjNil.createInstance()
        } catch (err) {
            interpreter.setEnvironment(prevEnvironment)
            interpreter.setInstance(prevInstance)

            if (err instanceof ReturnValue) {
                return err.value()
            }
            throw err
        }
    }
    value() {
        return `func (${this._parameters
            .map((parameter) => parameter.lexeme())
            .join(", ")}) { }`
    }
}

export class ReturnValue extends Error {
    private constructor(private _value: IObj) {
        super("returnValue")
    }

    static createInstance(value: IObj) {
        return new ReturnValue(value)
    }

    value() {
        return this._value
    }
}
