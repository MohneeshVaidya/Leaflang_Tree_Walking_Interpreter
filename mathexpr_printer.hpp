#ifndef MATHEXPR_PRINTER_HPP
#define MATHEXPR_PRINTER_HPP

#include "expr.hpp"
#include <vector>

class MathExprPrinter : public ExprVisitor {
private:
    auto evaluate(const Expr* expr) const -> LeafObject*;

public:
    auto print(const std::vector<const Expr*>& expressions) const -> void;

    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* override;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* override;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* override;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* override;
};

#endif
