#ifndef STMT_HPP
#define STMT_HPP

#include "expr.hpp"
#include "token.hpp"
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
};


// StmtVisitor forward declaration
class StmtVisitor;


// Stmt classes
class Stmt {
public:
    static auto delete_object(const Stmt* object) -> void;

    virtual ~Stmt() = default;
    virtual auto accept(const StmtVisitor* visitor) const -> void = 0;
    virtual auto type() const -> StmtType = 0;
};


// PrintStmt
class PrintStmt : public Stmt {
public:
    const Expr* expr;

private:
    PrintStmt(const Expr* expr);
    PrintStmt(const PrintStmt& source) = default;
    auto operator = (const PrintStmt& other) -> PrintStmt& = default;

public:
    ~PrintStmt();

    static auto create_object(const Expr* expr) -> PrintStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// PrintlnStmt
class PrintlnStmt : public Stmt {
public:
    const Expr* expr;

private:
    PrintlnStmt(const Expr* expr);
    PrintlnStmt(const PrintlnStmt& source) = default;
    auto operator = (const PrintlnStmt& other) -> PrintlnStmt& = default;

public:
    ~PrintlnStmt();

    static auto create_object(const Expr* expr) -> PrintlnStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// ExpressionStmt
class ExpressionStmt : public Stmt {
public:
    const Expr* expr;

private:
    ExpressionStmt(const Expr* expr);
    ExpressionStmt(const ExpressionStmt& source) = default;
    auto operator = (const ExpressionStmt& other) -> ExpressionStmt& = default;

public:
    ~ExpressionStmt();

    static auto create_object(const Expr* expr) -> ExpressionStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// VarStmt
class VarStmt : public Stmt {
public:
    const Token* identifier;
    const Expr* expr;

private:
    VarStmt(const Token* identifier, const Expr* expr);
    VarStmt(const VarStmt& source) = default;
    auto operator = (const VarStmt& other) -> VarStmt& = default;

public:
    ~VarStmt();

    static auto create_object(const Token* identifier, const Expr* expr) -> VarStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// ConstStmt
class ConstStmt : public Stmt {
public:
    const Token* identifier;
    const Expr* expr;

private:
    ConstStmt(const Token* identifier, const Expr* expr);
    ConstStmt(const ConstStmt& source) = default;
    auto operator = (const ConstStmt& other) -> ConstStmt& = default;

public:
    ~ConstStmt();

    static auto create_object(const Token* identifier, const Expr* expr) -> ConstStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// BlockStmt
class BlockStmt : public Stmt {
public:
    std::vector<const Stmt*> statements;

private:
    BlockStmt(const std::vector<const Stmt*>& statements);
    BlockStmt(const BlockStmt& source) = default;
    auto operator = (const BlockStmt& other) -> BlockStmt& = default;

public:
    ~BlockStmt();

    static auto create_object(const std::vector<const Stmt*>& statements) -> BlockStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// IfStmt
class IfStmt : public Stmt {
public:
    std::vector<std::pair<const Expr*, const Stmt*>> statements;

private:
    IfStmt(const std::vector<std::pair<const Expr*, const Stmt*>>& statements);

public:
    ~IfStmt();

    static auto create_object(const std::vector<std::pair<const Expr*, const Stmt*>>& statements) -> IfStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// ForStmt
class ForStmt : public Stmt {
public:
    const Expr* condition;
    const Stmt* block_stmt;

private:
    ForStmt(const Expr* condition, const Stmt* block_stmt);
    ForStmt(const ForStmt& source) = default;
    auto operator = (const ForStmt& source) -> ForStmt& = default;

public:
    ~ForStmt();

    static auto create_object(const Expr* condition, const Stmt* block_stmt) -> ForStmt*;

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;
};


// StmtVisitor
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual auto execute(const std::vector<const Stmt*>& statements) const -> void = 0;
    virtual auto visit_printstmt(const PrintStmt* stmt) const -> void = 0;
    virtual auto visit_printlnstmt(const PrintlnStmt* stmt) const -> void = 0;
    virtual auto visit_expressionstmt(const ExpressionStmt* stmt) const -> void = 0;
    virtual auto visit_varstmt(const VarStmt* stmt) const -> void = 0;
    virtual auto visit_conststmt(const ConstStmt* stmt) const -> void = 0;
    virtual auto visit_blockstmt(const BlockStmt* stmt) -> void = 0;
    virtual auto visit_ifstmt(const IfStmt* stmt) const -> void = 0;
    virtual auto visit_forstmt(const ForStmt* stmt) const -> void = 0;
};


#endif
