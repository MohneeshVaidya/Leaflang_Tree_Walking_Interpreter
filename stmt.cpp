#include "stmt.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "token.hpp"


// Stmt classes
auto Stmt::delete_object(Stmt* object) -> void {
    delete object;
}


// PrintStmt
auto PrintStmt::create_object(Expr* expr) -> PrintStmt* {
    return new PrintStmt { expr };
}

PrintStmt::PrintStmt(Expr* expr) :
    expr { expr }
    {
    }

PrintStmt::~PrintStmt() {
    Expr::delete_object(expr);
}

auto PrintStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_printstmt(this);
}

auto PrintStmt::type() -> StmtType {
    return StmtType::k_print;
}


// PrintlnStmt
auto PrintlnStmt::create_object(Expr* expr) -> PrintlnStmt* {
    return new PrintlnStmt { expr };
}

PrintlnStmt::PrintlnStmt(Expr* expr) :
    expr { expr }
    {
    }

PrintlnStmt::~PrintlnStmt() {
    Expr::delete_object(expr);
}

auto PrintlnStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_printlnstmt(this);
}

auto PrintlnStmt::type() -> StmtType {
    return StmtType::k_print;
}


// ExpressionStmt
auto ExpressionStmt::create_object(Expr* expr) -> ExpressionStmt* {
    return new ExpressionStmt { expr };
}

ExpressionStmt::ExpressionStmt(Expr* expr) :
    expr { expr }
    {
    }

ExpressionStmt::~ExpressionStmt() {
    Expr::delete_object(expr);
}

auto ExpressionStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_expressionstmt(this);
}

auto ExpressionStmt::type() -> StmtType {
    return StmtType::k_expression;
}


// VarStmt
auto VarStmt::create_object(Token* identifier, Expr* expr) -> VarStmt* {
    return new VarStmt { identifier, expr };
}

VarStmt::VarStmt(Token* identifier, Expr* expr) :
    identifier { Token::create_object(*identifier) },
    expr { expr }
    {
    }

VarStmt::~VarStmt() {
    Token::delete_object(identifier);
    Expr::delete_object(expr);
}

auto VarStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_varstmt(this);
}

auto VarStmt::type() -> StmtType {
    return StmtType::k_var;
}


// ConstStmt
auto ConstStmt::create_object(Token* identifier, Expr* expr) -> ConstStmt* {
    return new ConstStmt { identifier, expr };
}

ConstStmt::ConstStmt(Token* identifier, Expr* expr) :
    identifier { Token::create_object(*identifier) },
    expr { expr }
    {
    }

ConstStmt::~ConstStmt() {
    Token::delete_object(identifier);
    Expr::delete_object(expr);
}

auto ConstStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_conststmt(this);
}

auto ConstStmt::type() -> StmtType {
    return StmtType::k_const;
}


// BlockStmt
auto BlockStmt::create_object(std::vector<Stmt*>& statements) -> BlockStmt* {
    return new BlockStmt { statements };
}

BlockStmt::BlockStmt(std::vector<Stmt*>& statements) :
    statements { statements }
    {
    }

BlockStmt::~BlockStmt() {
    for (Stmt* stmt :statements) {
        Stmt::delete_object(stmt);
    }
}

auto BlockStmt::accept(StmtVisitor* visitor) -> void {
    return const_cast<StmtVisitor*>(visitor)->visit_blockstmt(this, Environment::create_object());
}

auto BlockStmt::type() -> StmtType {
    return StmtType::k_block;
}


// IfStmt
IfStmt::IfStmt(std::vector<std::pair<Expr*, Stmt*>>& statements) :
    statements { statements }
    {
    }

IfStmt::~IfStmt() {
    for (auto item : statements) {
        Expr::delete_object(item.first);
        Stmt::delete_object(item.second);
    }
}

auto IfStmt::create_object(std::vector<std::pair<Expr*, Stmt*>>& statements) -> IfStmt* {
    return new IfStmt { statements };
}

auto IfStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_ifstmt(this);
}

auto IfStmt::type() -> StmtType {
    return StmtType::k_if;
}


// ForStmt
ForStmt::ForStmt(Expr* condition, Stmt* block_stmt) :
    condition { condition },
    block_stmt { block_stmt }
    {
    }

ForStmt::~ForStmt() {
    Expr::delete_object(condition);
    Stmt::delete_object(block_stmt);
}

auto ForStmt::create_object(Expr* condition, Stmt* block_stmt) -> ForStmt* {
    return new ForStmt { condition, block_stmt };
}

auto ForStmt::accept(StmtVisitor* visitor) -> void {
    return const_cast<StmtVisitor*>(visitor)->visit_forstmt(this);
}

auto ForStmt::type() -> StmtType {
    return StmtType::k_for;
}


// BreakStmt
BreakStmt::BreakStmt(uint32_t line) :
    line { line }
    {
    }

auto BreakStmt::create_object(uint32_t line) -> BreakStmt* {
    return new BreakStmt { line };
}

auto BreakStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_breakstmt(this);
}

auto BreakStmt::type() -> StmtType {
    return StmtType::k_break;
}


// ContinueStmt
ContinueStmt::ContinueStmt(uint32_t line, Expr* step_expr) :
    line { line },
    step_expr { step_expr }
    {
    }

auto ContinueStmt::create_object(uint32_t line, Expr* step_expr) -> ContinueStmt* {
    return new ContinueStmt { line, step_expr };
}

auto ContinueStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_continuestmt(this);
}

auto ContinueStmt::type() -> StmtType {
    return StmtType::k_continue;
}


// ReturnStmt
ReturnStmt::ReturnStmt(Token* token, Expr* value) :
    token { token },
    value { value }
    {
    }

ReturnStmt::~ReturnStmt() {
    Expr::delete_object(value);
}

auto ReturnStmt::create_object(Token* token, Expr* value) -> ReturnStmt* {
    return new ReturnStmt { token, value };
}

auto ReturnStmt::accept(StmtVisitor* visitor) -> void {
    return visitor->visit_returnstmt(this);
}

auto ReturnStmt::type() -> StmtType {
    return StmtType::k_return;
}
