import binaryOperation from "./binaryOperation"
import Environment from "./environment"
import LeafError from "./error"
import IExpr, {
    AssignExpr,
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
import IStmt, {
    BlockStmt,
    BreakStmt,
    ConstStmt,
    ContinueStmt,
    ExprStmt,
    ForStmt,
    FuncStmt,
    IfStmt,
    InfiniteForStmt,
    IStmtVisitor,
    LetForStmt,
    LetStmt,
    PrintlnStmt,
    PrintStmt,
    ReturnStmt,
    WhileForStmt,
} from "./stmt"
import tokenType from "./tokenType"
import utils from "./utils"

export default class Interpreter implements IExprVisitor<IObj>, IStmtVisitor {
    private constructor(private _environment = Environment.createInstance()) {}

    static createInstance() {
        return new Interpreter()
    }

    execute(stmts: IStmt[]): void {
        stmts.forEach((stmt) => {
            this.executeStmt(stmt)
        })
    }

    // statement visitor functions
    private executeStmt(stmt: IStmt): void {
        stmt.accept(this)
    }

    visitPrintStmt(stmt: PrintStmt): void {
        process.stdout.write(`${this.evaluate(stmt.expr()).value()}`)
    }

    visitPrintlnStmt(stmt: PrintlnStmt): void {
        console.log(this.evaluate(stmt.expr()).value())
    }

    visitLetStmt(stmt: LetStmt): void {
        this._environment.insertLet(
            stmt.identifier(),
            this.evaluate(stmt.expr())
        )
    }

    visitConstStmt(stmt: ConstStmt): void {
        this._environment.insertConst(
            stmt.identifier(),
            this.evaluate(stmt.expr())
        )
    }

    visitBlockStmt(stmt: BlockStmt): void {
        const environment = Environment.createInstance(this._environment)
        this._environment = environment

        stmt.stmts().forEach((stmt) => {
            this.executeStmt(stmt)
        })

        this._environment = this._environment.parent() as Environment
    }

    visitIfStmt(stmt: IfStmt): void {
        throw new Error("Method not implemented.")
    }

    visitInfiniteForStmt(stmt: InfiniteForStmt): void {
        throw new Error("Method not implemented.")
    }

    visitWhileForStmt(stmt: WhileForStmt): void {
        throw new Error("Method not implemented.")
    }

    visitForStmt(stmt: ForStmt): void {
        throw new Error("Method not implemented.")
    }

    visitLetForStmt(stmt: LetForStmt): void {
        throw new Error("Method not implemented.")
    }

    visitBreakStmt(stmt: BreakStmt): void {
        throw new Error("Method not implemented.")
    }

    visitContinueStmt(stmt: ContinueStmt): void {
        throw new Error("Method not implemented.")
    }

    visitFuncStmt(stmt: FuncStmt): void {
        throw new Error("Method not implemented.")
    }

    visitReturnStmt(stmt: ReturnStmt): void {
        throw new Error("Method not implemented.")
    }

    visitExprStmt(stmt: ExprStmt): void {
        this.evaluate(stmt.expr())
    }

    // expression visitor functions
    private evaluate(expr: IExpr) {
        return expr.accept(this)
    }

    visitAssignExpr(expr: AssignExpr): IObj {
        const value = this.evaluate(expr.right())
        this._environment.set(expr.left().token(), value)
        return value
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
        return this._environment.get(expr.token())
    }

    visitNilExpr(expr: NilExpr): IObj {
        return ObjNil.createInstance()
    }
}
