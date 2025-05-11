#ifndef STMT_HPP
#define STMT_HPP

#include "environment.hpp"
#include "expr.hpp"
#include "token.hpp"

#include <cstdint>
#include <vector>


// StmtType enum
enum class StmtType {
    k_print,
    k_expression,
    k_var,
    k_const,
    k_block,
    k_if,
    k_for,
    k_break,
    k_continue,
    k_return,
};


// StmtVisitor forward declaration
class StmtVisitor;


// Stmt classes
class Stmt {
public:
    static auto delete_object(Stmt* object) -> void;

    virtual ~Stmt() = default;
    virtual auto accept(StmtVisitor* visitor) -> void = 0;
    virtual auto type() -> StmtType = 0;
};


// PrintStmt
class PrintStmt : public Stmt {
public:
    Expr* expr;

private:
    PrintStmt(Expr* expr);
    PrintStmt(PrintStmt& source) = default;
    auto operator = (PrintStmt& other) -> PrintStmt& = default;

public:
    ~PrintStmt();

    static auto create_object(Expr* expr) -> PrintStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// PrintlnStmt
class PrintlnStmt : public Stmt {
public:
    Expr* expr;

private:
    PrintlnStmt(Expr* expr);
    PrintlnStmt(PrintlnStmt& source) = default;
    auto operator = (PrintlnStmt& other) -> PrintlnStmt& = default;

public:
    ~PrintlnStmt();

    static auto create_object(Expr* expr) -> PrintlnStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// ExpressionStmt
class ExpressionStmt : public Stmt {
public:
    Expr* expr;

private:
    ExpressionStmt(Expr* expr);
    ExpressionStmt(ExpressionStmt& source) = default;
    auto operator = (ExpressionStmt& other) -> ExpressionStmt& = default;

public:
    ~ExpressionStmt();

    static auto create_object(Expr* expr) -> ExpressionStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// VarStmt
class VarStmt : public Stmt {
public:
    Token* identifier;
    Expr* expr;

private:
    VarStmt(Token* identifier, Expr* expr);
    VarStmt(VarStmt& source) = default;
    auto operator = (VarStmt& other) -> VarStmt& = default;

public:
    ~VarStmt();

    static auto create_object(Token* identifier, Expr* expr) -> VarStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// ConstStmt
class ConstStmt : public Stmt {
public:
    Token* identifier;
    Expr* expr;

private:
    ConstStmt(Token* identifier, Expr* expr);
    ConstStmt(ConstStmt& source) = default;
    auto operator = (ConstStmt& other) -> ConstStmt& = default;

public:
    ~ConstStmt();

    static auto create_object(Token* identifier, Expr* expr) -> ConstStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// BlockStmt
class BlockStmt : public Stmt {
public:
    std::vector<Stmt*> statements;

private:
    BlockStmt(std::vector<Stmt*>& statements);
    BlockStmt(BlockStmt& source) = default;
    auto operator = (BlockStmt& other) -> BlockStmt& = default;

public:
    ~BlockStmt();

    static auto create_object(std::vector<Stmt*>& statements) -> BlockStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// IfStmt
class IfStmt : public Stmt {
public:
    std::vector<std::pair<Expr*, Stmt*>> statements;

private:
    IfStmt(std::vector<std::pair<Expr*, Stmt*>>& statements);

public:
    ~IfStmt();

    static auto create_object(std::vector<std::pair<Expr*, Stmt*>>& statements) -> IfStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// ForStmt
class ForStmt : public Stmt {
public:
    Expr* condition;
    Stmt* block_stmt;

private:
    ForStmt(Expr* condition, Stmt* block_stmt);
    ForStmt(ForStmt& source) = default;
    auto operator = (ForStmt& source) -> ForStmt& = default;

public:
    ~ForStmt();

    static auto create_object(Expr* condition, Stmt* block_stmt) -> ForStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// BreakStmt
class BreakStmt : public Stmt {
public:
    uint32_t line;

private:
    BreakStmt(uint32_t line);

public:
    static auto create_object(uint32_t line) -> BreakStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// ContinueStmt
class ContinueStmt : public Stmt {
public:
    uint32_t line;
    Expr* step_expr;  // ContinueStmt is not responsible to deallocate the memory for 'step_expr'

private:
    ContinueStmt(uint32_t line, Expr* step_expr);
    ContinueStmt(ContinueStmt& source) = default;
    auto operator = (ContinueStmt&) -> ContinueStmt& { return *this; };

public:
    static auto create_object(uint32_t line, Expr* step_expr) -> ContinueStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// ReturnStmt
class ReturnStmt : public Stmt {
public:
    Token* token;
    Expr* value;

private:
    ReturnStmt(Token* token, Expr* value);
    ReturnStmt(ReturnStmt& source) = default;
    auto operator = (ReturnStmt& other) -> ReturnStmt& = default;

public:
    ~ReturnStmt();
    static auto create_object(Token* token, Expr* value) -> ReturnStmt*;

    virtual auto accept(StmtVisitor* visitor) -> void override;
    virtual auto type() -> StmtType override;
};


// StmtVisitor
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual auto execute(std::vector<Stmt*>& statements) -> void = 0;
    virtual auto visit_printstmt(PrintStmt* stmt) -> void = 0;
    virtual auto visit_printlnstmt(PrintlnStmt* stmt) -> void = 0;
    virtual auto visit_expressionstmt(ExpressionStmt* stmt) -> void = 0;
    virtual auto visit_varstmt(VarStmt* stmt) -> void = 0;
    virtual auto visit_conststmt(ConstStmt* stmt) -> void = 0;
    virtual auto visit_blockstmt(BlockStmt* stmt, Environment* environment) -> void = 0;
    virtual auto visit_ifstmt(IfStmt* stmt) -> void = 0;
    virtual auto visit_forstmt(ForStmt* stmt) -> void = 0;
    virtual auto visit_breakstmt(BreakStmt* stmt) -> void = 0;
    virtual auto visit_continuestmt(ContinueStmt* stmt) -> void = 0;
    virtual auto visit_returnstmt(ReturnStmt* stmt) -> void = 0;
};


#endif
