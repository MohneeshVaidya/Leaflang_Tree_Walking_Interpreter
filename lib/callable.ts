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

    call(args: IExpr[], interpreter: Interpreter, expr: IExpr): IObj {
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
