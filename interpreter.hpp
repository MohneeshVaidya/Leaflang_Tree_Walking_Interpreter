#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expr.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"

#include <vector>

class Interpreter : public ExprVisitor, public StmtVisitor {
private:
    auto execute_stmt(const Stmt* stmt) const -> void;
    auto evaluate(const Expr* expr) const -> LeafObject*;

public:
    virtual auto execute(const std::vector<const Stmt*>& statements) const -> void override;
    virtual auto execute(const std::vector<const Expr*>& expressions) const -> LeafObject* override;

    virtual auto visit_printstmt(const PrintStmt* stmt) const -> void override;
    virtual auto visit_expressionstmt(const ExpressionStmt* stmt) const -> void override;

    virtual auto visit_nullexpr(const NullExpr* expr) const -> LeafObject* override;
    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* override;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* override;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* override;
};

#endif
