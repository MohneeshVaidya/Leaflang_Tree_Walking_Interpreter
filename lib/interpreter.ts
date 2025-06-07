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
    ForWrapperStmt,
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

enum BlockCtx {
    NONE,
    IF,
    FOR,
}

const breakStmt = "breakStmt"
const continueStmt = "continueStmt"

export default class Interpreter implements IExprVisitor<IObj>, IStmtVisitor {
    private constructor(
        private _environment = Environment.createInstance(),
        private _blockCtx = BlockCtx.NONE
    ) {}

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

        try {
            stmt.stmts().forEach((stmt) => {
                this.executeStmt(stmt)
            })
        } catch (err) {
            this._environment = this._environment.parent() as Environment
            throw err
        }
        this._environment = this._environment.parent() as Environment
    }

    visitIfStmt(stmt: IfStmt): void {
        stmt.stmtTable().find(([condition, block]) => {
            if (utils.isTruthy(this.evaluate(condition))) {
                this.executeStmt(block)
                return true
            }
            return false
        })
    }

    visitForWrapperStmt(stmt: ForWrapperStmt): void {
        const prevBlockCtx = this._blockCtx
        this._blockCtx = BlockCtx.FOR
        try {
            this.executeStmt(stmt.forStmt())
        } catch (err) {
            if ((err as Error).message !== breakStmt) {
                throw err
            }
        }
        this._blockCtx = prevBlockCtx
    }

    visitInfiniteForStmt(stmt: InfiniteForStmt): void {
        while (true) {
            this.continueWrapper(stmt.stmts())
        }
    }

    visitWhileForStmt(stmt: WhileForStmt): void {
        while (utils.isTruthy(this.evaluate(stmt.condition()))) {
            this.continueWrapper(stmt.stmts())
        }
    }

    visitForStmt(stmt: ForStmt): void {
        for (
            this.evaluate(stmt.init());
            utils.isTruthy(this.evaluate(stmt.condition()));
            this.evaluate(stmt.step())
        ) {
            this.continueWrapper(stmt.stmts())
        }
    }

    visitLetForStmt(stmt: LetForStmt): void {
        const environment = Environment.createInstance(this._environment)
        this._environment = environment

        try {
            for (
                this.executeStmt(stmt.init());
                utils.isTruthy(this.evaluate(stmt.condition()));
                this.evaluate(stmt.step())
            ) {
                this.continueWrapper(stmt.stmts())
            }
        } catch (err) {
            this._environment = this._environment.parent() as Environment
            if ((err as Error).message !== breakStmt) {
                throw err
            }
        }
    }

    visitBreakStmt(stmt: BreakStmt): void {
        if (this._blockCtx !== BlockCtx.FOR) {
            LeafError.getInstance().throwRunTimeError(
                stmt.keyword().line(),
                "'break' can only be used inside a loop"
            )
        }
        throw new Error(breakStmt)
    }

    visitContinueStmt(stmt: ContinueStmt): void {
        if (this._blockCtx !== BlockCtx.FOR) {
            LeafError.getInstance().throwRunTimeError(
                stmt.keyword().line(),
                "'continue' can only be used inside a loop"
            )
        }
        throw new Error(continueStmt)
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

    // private helper functions
    private continueWrapper(stmt: BlockStmt) {
        try {
            this.visitBlockStmt(stmt)
        } catch (err) {
            if ((err as Error).message !== continueStmt) {
                throw err
            }
        }
    }
}
