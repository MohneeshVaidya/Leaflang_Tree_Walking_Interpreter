export default interface IStmt {
    accept(visitor: IStmtVisitor): void
}

export class Stmt {}

export class PrintStmt {}

export class PrintlnStmt {}

export class LetStmt {}

export class ConstStmt {}

export class IfStmt {}

export class ForStmt {}

export class BreakStmt {}

export class ContinueStmt {}

export class FuncStmt {}

export class ReturnStmt {}

export interface IStmtVisitor {}
