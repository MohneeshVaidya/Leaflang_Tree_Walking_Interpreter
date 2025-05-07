#include "stmt.hpp"
#include "expr.hpp"
#include "token.hpp"


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


// VarStmt
auto VarStmt::create_object(const Token* identifier, const Expr* expr) -> VarStmt* {
    return new VarStmt { identifier, expr };
}

VarStmt::VarStmt(const Token* identifier, const Expr* expr) :
    identifier { Token::create_object(*identifier) },
    expr { expr }
    {
    }

VarStmt::~VarStmt() {
    Token::delete_object(identifier);
    Expr::delete_object(expr);
}

auto VarStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_varstmt(this);
}

auto VarStmt::type() const -> StmtType {
    return StmtType::k_var;
}


// ConstStmt
auto ConstStmt::create_object(const Token* identifier, const Expr* expr) -> ConstStmt* {
    return new ConstStmt { identifier, expr };
}

ConstStmt::ConstStmt(const Token* identifier, const Expr* expr) :
    identifier { Token::create_object(*identifier) },
    expr { expr }
    {
    }

ConstStmt::~ConstStmt() {
    Token::delete_object(identifier);
    Expr::delete_object(expr);
}

auto ConstStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_conststmt(this);
}

auto ConstStmt::type() const -> StmtType {
    return StmtType::k_const;
}
