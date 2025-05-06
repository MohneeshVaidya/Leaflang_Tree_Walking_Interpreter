#ifndef STMT_HPP
#define STMT_HPP

#include "expr.hpp"
#include <vector>


// StmtType enum
enum class StmtType {
    k_print,
    k_expression,
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

public:
    static auto create_object(const Expr* expr) -> PrintStmt*;

    PrintStmt(const Expr* expr);
    PrintStmt(const PrintStmt& source) = default;

    ~PrintStmt();

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;

    auto operator = (const PrintStmt& other) -> PrintStmt& = default;
};


// ExpressionStmt
class ExpressionStmt : public Stmt {
public:
    const Expr* expr;

public:
    static auto create_object(const Expr* expr) -> ExpressionStmt*;

    ExpressionStmt(const Expr* expr);
    ExpressionStmt(const ExpressionStmt& source) = default;

    ~ExpressionStmt();

    virtual auto accept(const StmtVisitor* visitor) const -> void override;
    virtual auto type() const -> StmtType override;

    auto operator = (const ExpressionStmt& other) -> ExpressionStmt& = default;
};


// StmtVisitor
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual auto execute(const std::vector<const Stmt*>& statements) const -> void = 0;
    virtual auto visit_printstmt(const PrintStmt* stmt) const -> void = 0;
    virtual auto visit_expressionstmt(const ExpressionStmt* stmt) const -> void = 0;
};


#endif
