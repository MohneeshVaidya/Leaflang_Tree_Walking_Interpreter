import { LeafInstance } from "./class"
import Environment from "./environment"
import LeafError from "./error"
import IExpr from "./expr"
import Interpreter from "./interpreter"
import IObj, { ObjNil } from "./object"
import { BlockStmt } from "./stmt"
import Token from "./token"

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
            LeafError.getInstance().throwRunTimeError(
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
    private _type: "method" | "make"

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

    call(args: IExpr[], interpreter: Interpreter, expr: IExpr): IObj {
        const prevEnvironment = this.execSharedCallCode(args, interpreter, expr)
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
