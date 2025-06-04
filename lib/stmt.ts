import IExpr from "./expr"
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

export class ForStmt implements IStmt {
    accept(visitor: IStmtVisitor): void {
        visitor.visitForStmt(this)
    }
}

export class BreakStmt implements IStmt {
    accept(visitor: IStmtVisitor): void {
        visitor.visitBreakStmt(this)
    }
}

export class ContinueStmt implements IStmt {
    accept(visitor: IStmtVisitor): void {
        visitor.visitContinueStmt(this)
    }
}

export class FuncStmt implements IStmt {
    accept(visitor: IStmtVisitor): void {
        visitor.visitFuncStmt(this)
    }
}

export class ReturnStmt implements IStmt {
    accept(visitor: IStmtVisitor): void {
        visitor.visitFuncStmt(this)
    }
}

export class ExprStmt implements IStmt {
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
    visitForStmt(stmt: ForStmt): void
    visitBreakStmt(stmt: BreakStmt): void
    visitContinueStmt(stmt: ContinueStmt): void
    visitFuncStmt(stmt: FuncStmt): void
    visitReturnStmt(stmt: ReturnStmt): void
    visitExprStmt(stmt: ExprStmt): void
}
