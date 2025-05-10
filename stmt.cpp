#include "stmt.hpp"
#include "environment.hpp"
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


// PrintlnStmt
auto PrintlnStmt::create_object(const Expr* expr) -> PrintlnStmt* {
    return new PrintlnStmt { expr };
}

PrintlnStmt::PrintlnStmt(const Expr* expr) :
    expr { expr }
    {
    }

PrintlnStmt::~PrintlnStmt() {
    Expr::delete_object(expr);
}

auto PrintlnStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_printlnstmt(this);
}

auto PrintlnStmt::type() const -> StmtType {
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


// BlockStmt
auto BlockStmt::create_object(const std::vector<const Stmt*>& statements) -> BlockStmt* {
    return new BlockStmt { statements };
}

BlockStmt::BlockStmt(const std::vector<const Stmt*>& statements) :
    statements { statements }
    {
    }

BlockStmt::~BlockStmt() {
    for (const Stmt* stmt :statements) {
        Stmt::delete_object(stmt);
    }
}

auto BlockStmt::accept(const StmtVisitor* visitor) const -> void {
    return const_cast<StmtVisitor*>(visitor)->visit_blockstmt(this, Environment::create_object());
}

auto BlockStmt::type() const -> StmtType {
    return StmtType::k_block;
}


// IfStmt
IfStmt::IfStmt(const std::vector<std::pair<const Expr*, const Stmt*>>& statements) :
    statements { statements }
    {
    }

IfStmt::~IfStmt() {
    for (auto item : statements) {
        Expr::delete_object(item.first);
        Stmt::delete_object(item.second);
    }
}

auto IfStmt::create_object(const std::vector<std::pair<const Expr*, const Stmt*>>& statements) -> IfStmt* {
    return new IfStmt { statements };
}

auto IfStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_ifstmt(this);
}

auto IfStmt::type() const -> StmtType {
    return StmtType::k_if;
}


// ForStmt
ForStmt::ForStmt(const Expr* condition, const Stmt* block_stmt) :
    condition { condition },
    block_stmt { block_stmt }
    {
    }

ForStmt::~ForStmt() {
    Expr::delete_object(condition);
    Stmt::delete_object(block_stmt);
}

auto ForStmt::create_object(const Expr* condition, const Stmt* block_stmt) -> ForStmt* {
    return new ForStmt { condition, block_stmt };
}

auto ForStmt::accept(const StmtVisitor* visitor) const -> void {
    return const_cast<StmtVisitor*>(visitor)->visit_forstmt(this);
}

auto ForStmt::type() const -> StmtType {
    return StmtType::k_for;
}


// BreakStmt
BreakStmt::BreakStmt(const uint32_t line) :
    line { line }
    {
    }

auto BreakStmt::create_object(const uint32_t line) -> BreakStmt* {
    return new BreakStmt { line };
}

auto BreakStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_breakstmt(this);
}

auto BreakStmt::type() const -> StmtType {
    return StmtType::k_break;
}


// ContinueStmt
ContinueStmt::ContinueStmt(const uint32_t line, const Expr* step_expr) :
    line { line },
    step_expr { step_expr }
    {
    }

auto ContinueStmt::create_object(const uint32_t line, const Expr* step_expr) -> ContinueStmt* {
    return new ContinueStmt { line, step_expr };
}

auto ContinueStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_continuestmt(this);
}

auto ContinueStmt::type() const -> StmtType {
    return StmtType::k_continue;
}


// ReturnStmt
ReturnStmt::ReturnStmt(const Token* token, const Expr* value) :
    token { token },
    value { value }
    {
    }

ReturnStmt::~ReturnStmt() {
    Expr::delete_object(value);
}

auto ReturnStmt::create_object(const Token* token, const Expr* value) -> ReturnStmt* {
    return new ReturnStmt { token, value };
}

auto ReturnStmt::accept(const StmtVisitor* visitor) const -> void {
    return visitor->visit_returnstmt(this);
}

auto ReturnStmt::type() const -> StmtType {
    return StmtType::k_return;
}
