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

export default class Printer implements IExprVisitor<void> {
    private constructor() {}

    static createInstance() {
        return new Printer()
    }

    execute(exprs: IExpr[]): void {
        exprs.forEach((expr) => {
            this.evaluate(expr)
            console.log()
        })
    }

    private evaluate(expr: IExpr) {
        expr.accept(this)
    }

    visitTernaryExpr(expr: TernaryExpr): void {
        process.stdout.write("( ")
        this.evaluate(expr.condition())
        process.stdout.write("? ")
        this.evaluate(expr.first())
        process.stdout.write(": ")
        this.evaluate(expr.second())
        process.stdout.write(") ")
    }
    visitBinaryExpr(expr: BinaryExpr): void {
        process.stdout.write("( ")
        this.evaluate(expr.left())
        process.stdout.write(expr.oper().lexeme() + " ")
        this.evaluate(expr.right())
        process.stdout.write(") ")
    }
    visitExponentExpr(expr: ExponentExpr): void {
        process.stdout.write("( ")
        this.evaluate(expr.left())
        process.stdout.write("** ")
        this.evaluate(expr.right())
        process.stdout.write(") ")
    }
    visitUnaryExpr(expr: UnaryExpr): void {
        process.stdout.write("( " + expr.oper().lexeme() + " ")
        this.evaluate(expr.expr())
    }
    visitGroupingExpr(expr: GroupingExpr): void {
        process.stdout.write("( ")
        this.evaluate(expr.expr())
        process.stdout.write(") ")
    }
    visitLiteralExpr(expr: LiteralExpr): void {
        process.stdout.write(expr.token().lexeme() + " ")
    }
    visitIdentifierExpr(expr: IdentifierExpr): void {
        process.stdout.write(expr.token().lexeme() + " ")
    }
    visitNilExpr(expr: NilExpr): void {
        process.stdout.write("nil ")
    }
}
