#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "environment.hpp"
#include "expr.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"

#include <vector>


// BlockCtx
enum class BlockCtx {
    k_none,
    k_loop,
};

// FunctionCtx
enum class FunctionCtx {
    k_none,
    k_function,
};


// Interpreter
class Interpreter : public ExprVisitor, public StmtVisitor {
    friend class LeafFunction;

private:
    Environment* m_environment { };
    BlockCtx block_ctx { BlockCtx::k_none };
    FunctionCtx func_ctx { FunctionCtx::k_none };

    auto execute_stmt(const Stmt* stmt) const -> void;
    auto evaluate(const Expr* expr) const -> LeafObject*;

    auto operator = (const Interpreter& other) -> Interpreter& = default;

public:
    Interpreter(Environment* environment);
    Interpreter(const Interpreter& source) = default;
    ~Interpreter();

    virtual auto execute(const std::vector<const Stmt*>& statements) const -> void override;
    virtual auto execute(const std::vector<const Expr*>& expressions) const -> LeafObject* override;

    virtual auto visit_printstmt(const PrintStmt* stmt) const -> void override;
    virtual auto visit_printlnstmt(const PrintlnStmt* stmt) const -> void override;
    virtual auto visit_expressionstmt(const ExpressionStmt* stmt) const -> void override;
    virtual auto visit_varstmt(const VarStmt* stmt) const -> void override;
    virtual auto visit_conststmt(const ConstStmt* stmt) const -> void override;
    virtual auto visit_blockstmt(const BlockStmt* stmt, Environment* environment) -> void override;
    virtual auto visit_ifstmt(const IfStmt* stmt) const -> void override;
    virtual auto visit_forstmt(const ForStmt* stmt) -> void override;
    virtual auto visit_breakstmt(const BreakStmt* stmt) const -> void override;
    virtual auto visit_continuestmt(const ContinueStmt* stmt) const -> void override;
    virtual auto visit_returnstmt(const ReturnStmt* stmt) const -> void override;

    virtual auto visit_nullexpr(const NullExpr* expr) const -> LeafObject* override;
    virtual auto visit_assignexpr(const AssignExpr* expr) const -> LeafObject* override;
    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* override;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* override;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_functionexpr(const FunctionExpr* expr) const -> LeafObject* override;
    virtual auto visit_callexpr(const CallExpr* expr) -> LeafObject* override;
};

#endif
