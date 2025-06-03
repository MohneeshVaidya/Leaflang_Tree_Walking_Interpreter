import Token from "./token"

export default interface IExpr {
    accept<T>(visitor: IExprVisitor<T>): T
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

export interface IExprVisitor<T> {
    visitTernaryExpr(expr: TernaryExpr): T
    visitBinaryExpr(expr: BinaryExpr): T
    visitExponentExpr(expr: ExponentExpr): T
    visitUnaryExpr(expr: UnaryExpr): T
    visitGroupingExpr(expr: GroupingExpr): T
    visitLiteralExpr(expr: LiteralExpr): T
    visitIdentifierExpr(expr: IdentifierExpr): T
}
