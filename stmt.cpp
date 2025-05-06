#include "stmt.hpp"
#include "expr.hpp"


// Stmt classes
auto Stmt::delete_object(const Stmt* object) -> void {
    delete object;
}


// PrintStmt
auto PrintStmt::create_object(const Expr* expr) -> PrintStmt* {
    return new PrintStmt { expr };
}

PrintStmt::PrintStmt(const Expr* expr) :
    expr { expr }
    {
    }

PrintStmt::~PrintStmt() {
    Expr::delete_object(expr);
}

auto PrintStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_printstmt(this);
}

auto PrintStmt::type() const -> StmtType {
    return StmtType::k_print;
}


// ExpressionStmt
auto ExpressionStmt::create_object(const Expr* expr) -> ExpressionStmt* {
    return new ExpressionStmt { expr };
}

ExpressionStmt::ExpressionStmt(const Expr* expr) :
    expr { expr }
    {
    }

ExpressionStmt::~ExpressionStmt() {
    Expr::delete_object(expr);
}

auto ExpressionStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_expressionstmt(this);
}

auto ExpressionStmt::type() const -> StmtType {
    return StmtType::k_expression;
}
