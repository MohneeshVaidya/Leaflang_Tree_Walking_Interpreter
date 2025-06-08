import { BlockStmt } from "./stmt"
import Token from "./token"

export default interface IExpr {
    accept<T>(visitor: IExprVisitor<T>): T
}

export class AssignExpr implements IExpr {
    constructor(
        private _oper: Token,
        private _left: IdentifierExpr,
        private _right: IExpr
    ) {}

    static createInstance(oper: Token, left: IExpr, right: IExpr) {
        return new AssignExpr(oper, left as IdentifierExpr, right)
    }

    oper() {
        return this._oper
    }

    left() {
        return this._left
    }

    right() {
        return this._right
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitAssignExpr(this)
    }
}

export class TernaryExpr implements IExpr {
    private constructor(
        private _condition: IExpr,
        private _operQuest: Token,
        private _first: IExpr,
        private _operColon: Token,
        private _second: IExpr
    ) {}

    static createInstance(
        condition: IExpr,
        operQuest: Token,
        first: IExpr,
        operColon: Token,
        second: IExpr
    ) {
        return new TernaryExpr(condition, operQuest, first, operColon, second)
    }

    condition() {
        return this._condition
    }

    operQuest() {
        return this._operQuest
    }

    first() {
        return this._first
    }

    operColon() {
        return this._operColon
    }

    second() {
        return this._second
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitTernaryExpr(this)
    }
}

export class BinaryExpr implements IExpr {
    private constructor(
        private _oper: Token,
        private _left: IExpr,
        private _right: IExpr
    ) {}

    static createInstance(oper: Token, left: IExpr, right: IExpr) {
        return new BinaryExpr(oper, left, right)
    }

    oper() {
        return this._oper
    }

    left() {
        return this._left
    }

    right() {
        return this._right
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitBinaryExpr(this)
    }
}

export class ExponentExpr implements IExpr {
    private constructor(
        private _oper: Token,
        private _left: IExpr,
        private _right: IExpr
    ) {}

    static createInstance(oper: Token, left: IExpr, right: IExpr) {
        return new ExponentExpr(oper, left, right)
    }

    oper() {
        return this._oper
    }

    left() {
        return this._left
    }

    right() {
        return this._right
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitExponentExpr(this)
    }
}

export class UnaryExpr implements IExpr {
    private constructor(private _oper: Token, private _expr: IExpr) {}

    static createInstance(oper: Token, expr: IExpr) {
        return new UnaryExpr(oper, expr)
    }

    oper() {
        return this._oper
    }

    expr() {
        return this._expr
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitUnaryExpr(this)
    }
}

export class FuncExpr implements IExpr {
    private constructor(
        private _parameters: Token[],
        private _stmts: BlockStmt
    ) {}

    static createInstance(parameters: Token[], stmts: BlockStmt) {
        return new FuncExpr(parameters, stmts)
    }

    parameters() {
        return this._parameters
    }

    stmts() {
        return this._stmts
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitFuncExpr(this)
    }
}

export class CallExpr implements IExpr {
    private constructor(private _caller: IExpr, private _args: IExpr[]) {}

    static createInstance(caller: IExpr, args: IExpr[]) {
        return new CallExpr(caller, args)
    }

    caller() {
        return this._caller
    }

    args() {
        return this._args
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitCallExpr(this)
    }
}

export class GetExpr implements IExpr {
    private constructor(private _caller: IExpr, private _name: Token) {}

    static createInstance(caller: IExpr, name: Token) {
        return new GetExpr(caller, name)
    }

    caller() {
        return this._caller
    }

    name() {
        return this._name
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitGetExpr(this)
    }
}

export class SetExpr implements IExpr {
    accept<T>(visitor: IExprVisitor<T>): T {
        throw new Error("Method not implemented.")
    }
}

export class ClassExpr implements IExpr {
    private constructor(
        private _name: Token,
        private _fields: Set<string>,
        private _methods: Map<string, FuncExpr>
    ) {}

    static createInstance(
        name: Token,
        fields: Set<string>,
        methods: Map<string, FuncExpr>
    ) {
        return new ClassExpr(name, fields, methods)
    }

    name() {
        return this._name
    }

    fields() {
        return this._fields
    }

    methods() {
        return this._methods
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitClassExpr(this)
    }
}

export class GroupingExpr implements IExpr {
    private constructor(private _leftParen: Token, private _expr: IExpr) {}

    static createInstance(leftParen: Token, expr: IExpr) {
        return new GroupingExpr(leftParen, expr)
    }

    leftParen() {
        return this._leftParen
    }

    expr() {
        return this._expr
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitGroupingExpr(this)
    }
}

export class LiteralExpr implements IExpr {
    private constructor(private _token: Token) {}

    static createInstance(_token: Token) {
        return new LiteralExpr(_token)
    }

    token() {
        return this._token
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitLiteralExpr(this)
    }
}

export class IdentifierExpr implements IExpr {
    private constructor(private _token: Token) {}

    static createInstance(token: Token) {
        return new IdentifierExpr(token)
    }

    token() {
        return this._token
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitIdentifierExpr(this)
    }
}

export class NilExpr implements IExpr {
    private constructor() {}

    static createInstance() {
        return new NilExpr()
    }

    accept<T>(visitor: IExprVisitor<T>): T {
        return visitor.visitNilExpr(this)
    }
}

export interface IExprVisitor<T> {
    visitAssignExpr(expr: AssignExpr): T
    visitTernaryExpr(expr: TernaryExpr): T
    visitBinaryExpr(expr: BinaryExpr): T
    visitExponentExpr(expr: ExponentExpr): T
    visitUnaryExpr(expr: UnaryExpr): T
    visitFuncExpr(expr: FuncExpr): T
    visitCallExpr(expr: CallExpr): T
    visitGetExpr(expr: GetExpr): T
    visitClassExpr(expr: ClassExpr): T
    visitGroupingExpr(expr: GroupingExpr): T
    visitLiteralExpr(expr: LiteralExpr): T
    visitIdentifierExpr(expr: IdentifierExpr): T
    visitNilExpr(expr: NilExpr): T
}
