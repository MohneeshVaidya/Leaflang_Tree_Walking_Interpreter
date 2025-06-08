import binaryOperation from "./binaryOperation"
import Callable, { LeafFunction, LeafMethod, ReturnValue } from "./callable"
import LeafClass, { LeafInstance } from "./class"
import Environment from "./environment"
import LeafError from "./error"
import IExpr, {
    AssignExpr,
    BinaryExpr,
    CallExpr,
    ClassExpr,
    ExponentExpr,
    FuncExpr,
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
    ClassStmt,
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
import Token from "./token"
import tokenType from "./tokenType"
import utils from "./utils"

enum BlockCtx {
    NONE,
    FOR,
}

enum CallableCtx {
    NONE,
    FUNCT,
    METHOD,
    MAKE,
}

const breakStmt = "breakStmt"
const continueStmt = "continueStmt"

export default class Interpreter implements IExprVisitor<IObj>, IStmtVisitor {
    private constructor(
        private _environment = Environment.createInstance(),
        private _blockCtx = BlockCtx.NONE,
        private _callableCtx = CallableCtx.NONE
    ) {}

    static createInstance() {
        return new Interpreter()
    }

    environment() {
        return this._environment
    }

    setEnvironment(environment: Environment) {
        this._environment = environment
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
        this._environment.insertConst(
            stmt.name() as Token,
            this.evaluate(stmt.expr())
        )
    }

    visitReturnStmt(stmt: ReturnStmt): void {
        if (this._callableCtx === CallableCtx.NONE) {
            LeafError.getInstance().throwRunTimeError(
                stmt.keyword().line(),
                "'return' statement can only be used inside a function"
            )
        }
        throw ReturnValue.createInstance(this.evaluate(stmt.value()))
    }

    visitClassStmt(stmt: ClassStmt): void {
        this._environment.insertConst(stmt.name(), this.evaluate(stmt.expr()))
    }

    visitExprStmt(stmt: ExprStmt): void {
        this.evaluate(stmt.expr())
    }

    // expression visitor functions
    evaluate(expr: IExpr) {
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

    visitFuncExpr(expr: FuncExpr): IObj {
        return LeafFunction.createInstance(
            expr.parameters(),
            expr.stmts(),
            this._environment
        )
    }

    visitCallExpr(expr: CallExpr): IObj {
        const caller = this.evaluate(expr.caller())

        if (!(caller instanceof Callable)) {
            if (caller instanceof LeafClass) {
                return this.callMake(caller, expr)
            }

            LeafError.getInstance().throwRunTimeError(
                this.getCallLine(expr),
                "expression is not callable"
            )
        }

        const prevCtx = this._callableCtx
        this._callableCtx = CallableCtx.FUNCT

        const value = (caller as unknown as Callable).call(
            expr.args(),
            this,
            expr
        )

        this._callableCtx = prevCtx
        return value
    }

    visitClassExpr(expr: ClassExpr): IObj {
        return LeafClass.createInstance(
            expr.fields(),
            Array.from(expr.methods().entries()).reduce(
                (prev, [key, value]) => {
                    prev.set(
                        key,
                        LeafMethod.createInstance(
                            value.parameters(),
                            value.stmts(),
                            this._environment
                        )
                    )
                    return prev
                },
                new Map()
            )
        )
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

    // helper functions
    private continueWrapper(stmt: BlockStmt) {
        try {
            this.visitBlockStmt(stmt)
        } catch (err) {
            if ((err as Error).message !== continueStmt) {
                throw err
            }
        }
    }

    getCallLine(expr: IExpr) {
        if (expr instanceof IdentifierExpr) {
            return expr.token().line()
        }
        return this.getCallLine((expr as CallExpr).caller())
    }

    private callMake(leafClass: LeafClass, expr: CallExpr): IObj {
        if (!leafClass.methods().has("constructor")) {
            const table = new Map()

            leafClass.fields().forEach((field) => {
                table.set(field, ObjNil.createInstance())
            })
            return LeafInstance.createInstance(table)
        }

        const make = leafClass.methods().get("constructor") as LeafMethod
        return make.call(expr.args(), this, expr)
    }
}
