import binaryOperation from "./binaryOperation"
import LeafError from "./error"
import IExpr, {
    BinaryExpr,
    ExponentExpr,
    GroupingExpr,
    IdentifierExpr,
    IExprVisitor,
    LiteralExpr,
    NilExpr,
    TernaryExpr,
    UnaryExpr,
} from "./expr"
import IObj, { ObjBool, ObjNil, ObjNumber, ObjString } from "./object"
import tokenType from "./tokenType"
import utils from "./utils"

export default class Interpreter implements IExprVisitor<IObj> {
    private constructor() {}

    static createInstance() {
        return new Interpreter()
    }

    execute(exprs: IExpr[]): void {
        exprs.forEach((expr) => {
            console.log(this.evaluate(expr).value())
        })
    }

    private evaluate(expr: IExpr) {
        return expr.accept(this)
    }

    visitTernaryExpr(expr: TernaryExpr): IObj {
        const condition = this.evaluate(expr.condition())
        if (utils.isTruthy(condition)) {
            return this.evaluate(expr.first())
        } else {
            return this.evaluate(expr.second())
        }
    }

    visitBinaryExpr(expr: BinaryExpr): IObj {
        const oper = expr.oper()
        const left = this.evaluate(expr.left())
        const right = this.evaluate(expr.right())
        return binaryOperation(oper, left, right)
    }

    visitExponentExpr(expr: ExponentExpr): IObj {
        const oper = expr.oper()
        const left = this.evaluate(expr.left())
        const right = this.evaluate(expr.right())
        return binaryOperation(oper, left, right)
    }

    visitUnaryExpr(expr: UnaryExpr): IObj {
        const oper = expr.oper()
        const expr_ = this.evaluate(expr.expr())
        if (oper.lexeme() === "!") {
            return ObjBool.createInstance(!utils.isTruthy(expr_))
        }
        if (expr_ instanceof ObjNumber) {
            return ObjNumber.createInstance(-expr_.value())
        }
        LeafError.getInstance().throwRunTimeError(
            oper.line(),
            "operand of unary '-' can only be a number;"
        )
        throw 0
    }

    visitGroupingExpr(expr: GroupingExpr): IObj {
        return this.evaluate(expr.expr())
    }

    visitLiteralExpr(expr: LiteralExpr): IObj {
        switch (expr.token().type()) {
            case tokenType.NUMBER:
                return ObjNumber.createInstance(
                    parseFloat(expr.token().lexeme())
                )
            case tokenType.STRING:
                return ObjString.createInstance(expr.token().lexeme())
            case tokenType.TRUE:
                return ObjBool.createInstance(true)
            case tokenType.FALSE:
                return ObjBool.createInstance(false)
        }
        throw "undefined literal"
    }

    visitIdentifierExpr(expr: IdentifierExpr): IObj {
        throw new Error("method not implemented.")
    }

    visitNilExpr(expr: NilExpr): IObj {
        return ObjNil.createInstance()
    }
}
