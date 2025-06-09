import IExpr, { ClassExpr, FuncExpr } from "./expr"
import Token from "./token"

export default interface IStmt {
    accept(visitor: IStmtVisitor): void
}

export class PrintStmt implements IStmt {
    private constructor(private _expr: IExpr) {}

    static createInstance(expr: IExpr) {
        return new PrintStmt(expr)
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitPrintStmt(this)
    }
}

export class PrintlnStmt implements IStmt {
    private constructor(private _expr: IExpr) {}

    static createInstance(expr: IExpr) {
        return new PrintlnStmt(expr)
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitPrintlnStmt(this)
    }
}

export class LetStmt implements IStmt {
    private constructor(private _identifier: Token, private _expr: IExpr) {}

    static createInstance(identifier: Token, expr: IExpr) {
        return new LetStmt(identifier, expr)
    }

    identifier() {
        return this._identifier
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitLetStmt(this)
    }
}

export class ConstStmt implements IStmt {
    private constructor(private _identifier: Token, private _expr: IExpr) {}

    static createInstance(identifier: Token, expr: IExpr) {
        return new ConstStmt(identifier, expr)
    }

    identifier() {
        return this._identifier
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitConstStmt(this)
    }
}

export class BlockStmt implements IStmt {
    private constructor(private _stmts: IStmt[]) {}

    static createInstance(stmts: IStmt[]) {
        return new BlockStmt(stmts)
    }

    stmts() {
        return this._stmts
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitBlockStmt(this)
    }
}

export class IfStmt implements IStmt {
    private constructor(private _stmtTable: [IExpr, BlockStmt][]) {}

    static createInstance(stmtTable: [IExpr, BlockStmt][]) {
        return new IfStmt(stmtTable)
    }

    stmtTable() {
        return this._stmtTable
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitIfStmt(this)
    }
}

export class ForWrapperStmt implements IStmt {
    private constructor(private _forStmt: IStmt) {}

    static createInstance(forStmt: IStmt) {
        return new ForWrapperStmt(forStmt)
    }

    forStmt() {
        return this._forStmt
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitForWrapperStmt(this)
    }
}

export class InfiniteForStmt implements IStmt {
    private constructor(private _stmts: BlockStmt) {}

    static createInstance(stmts: BlockStmt) {
        return new InfiniteForStmt(stmts)
    }

    stmts() {
        return this._stmts
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitInfiniteForStmt(this)
    }
}

export class WhileForStmt implements IStmt {
    private constructor(private _condition: IExpr, private _stmts: BlockStmt) {}

    static createInstance(condition: IExpr, stmts: BlockStmt) {
        return new WhileForStmt(condition, stmts)
    }

    condition() {
        return this._condition
    }

    stmts() {
        return this._stmts
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitWhileForStmt(this)
    }
}

export class ForStmt implements IStmt {
    private constructor(
        private _init: IExpr,
        private _condition: IExpr,
        private _step: IExpr,
        private _stmts: BlockStmt
    ) {}

    static createInstance(
        init: IExpr,
        condition: IExpr,
        step: IExpr,
        stmts: BlockStmt
    ) {
        return new ForStmt(init, condition, step, stmts)
    }

    init() {
        return this._init
    }

    condition() {
        return this._condition
    }

    step() {
        return this._step
    }

    stmts() {
        return this._stmts
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitForStmt(this)
    }
}

export class LetForStmt implements IStmt {
    private constructor(
        private _init: LetStmt,
        private _condition: IExpr,
        private _step: IExpr,
        private _stmts: BlockStmt
    ) {}

    static createInstance(
        init: LetStmt,
        condition: IExpr,
        step: IExpr,
        stmts: BlockStmt
    ) {
        return new LetForStmt(init, condition, step, stmts)
    }

    init() {
        return this._init
    }

    condition() {
        return this._condition
    }

    step() {
        return this._step
    }

    stmts() {
        return this._stmts
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitLetForStmt(this)
    }
}

export class BreakStmt implements IStmt {
    private constructor(private _keyword: Token) {}

    static createInstance(keyword: Token) {
        return new BreakStmt(keyword)
    }

    keyword() {
        return this._keyword
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitBreakStmt(this)
    }
}

export class ContinueStmt implements IStmt {
    private constructor(private _keyword: Token) {}

    static createInstance(keyword: Token) {
        return new ContinueStmt(keyword)
    }

    keyword() {
        return this._keyword
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitContinueStmt(this)
    }
}

export class FuncStmt implements IStmt {
    private constructor(private _name: Token, private _expr: FuncExpr) {}

    static createInstance(name: Token, expr: FuncExpr) {
        return new FuncStmt(name, expr)
    }

    name() {
        return this._name
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitFuncStmt(this)
    }
}

export class ReturnStmt implements IStmt {
    private constructor(private _keyword: Token, private _value: IExpr) {}

    static createInstance(keyword: Token, value: IExpr) {
        return new ReturnStmt(keyword, value)
    }

    keyword() {
        return this._keyword
    }

    value() {
        return this._value
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitReturnStmt(this)
    }
}

export class ClassStmt implements IStmt {
    private constructor(private _name: Token, private _expr: ClassExpr) {}

    static createInstance(name: Token, expr: ClassExpr) {
        return new ClassStmt(name, expr)
    }

    name() {
        return this._name
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitClassStmt(this)
    }
}

export class ExprStmt implements IStmt {
    private constructor(private _expr: IExpr) {}

    static createInstance(expr: IExpr) {
        return new ExprStmt(expr)
    }

    expr() {
        return this._expr
    }

    accept(visitor: IStmtVisitor): void {
        visitor.visitExprStmt(this)
    }
}

export interface IStmtVisitor {
    execute(stmts: IStmt[]): void

    visitPrintStmt(stmt: PrintStmt): void
    visitPrintlnStmt(stmt: PrintlnStmt): void
    visitLetStmt(stmt: LetStmt): void
    visitConstStmt(stmt: ConstStmt): void
    visitBlockStmt(stmt: BlockStmt): void
    visitIfStmt(stmt: IfStmt): void
    visitForWrapperStmt(stmt: ForWrapperStmt): void
    visitInfiniteForStmt(stmt: InfiniteForStmt): void
    visitWhileForStmt(stmt: WhileForStmt): void
    visitForStmt(stmt: ForStmt): void
    visitLetForStmt(stmt: LetForStmt): void
    visitBreakStmt(stmt: BreakStmt): void
    visitContinueStmt(stmt: ContinueStmt): void
    visitFuncStmt(stmt: FuncStmt): void
    visitReturnStmt(stmt: ReturnStmt): void
    visitClassStmt(stmt: ClassStmt): void
    visitExprStmt(stmt: ExprStmt): void
}
