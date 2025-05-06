#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expr.hpp"
#include "leaf_object.hpp"

#include <vector>

class Interpreter : public ExprVisitor {
private:
    auto evaluate(const Expr* expr) const -> LeafObject*;

public:
    virtual auto execute(const std::vector<const Expr*>& expressions) const -> LeafObject* override;
    virtual auto visit_nullexpr(const NullExpr* expr) const -> LeafObject* override;
    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* override;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* override;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* override;
};

#endif
