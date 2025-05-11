#ifndef EXPR_PRINTER_HPP
#define EXPR_PRINTER_HPP

#include "expr.hpp"
#include "leaf_object.hpp"

#include <vector>

class ExprPrinter : public ExprVisitor {
private:
    auto evaluate(Expr* expr) -> LeafObject*;

public:
    virtual auto execute(std::vector<Expr*>& expressions) -> LeafObject* override;
    virtual auto visit_nullexpr(NullExpr* expr) -> LeafObject* override;
    virtual auto visit_ternaryexpr(TernaryExpr* expr) -> LeafObject* override;
    virtual auto visit_binaryexpr(BinaryExpr* expr) -> LeafObject* override;
    virtual auto visit_unaryexpr(UnaryExpr* expr) -> LeafObject* override;
    virtual auto visit_exponentexpr(ExponentExpr* expr) -> LeafObject* override;
    virtual auto visit_groupingexpr(GroupingExpr* expr) -> LeafObject* override;
    virtual auto visit_primaryexpr(PrimaryExpr* expr) -> LeafObject* override;
};

#endif
