#ifndef leaf_printer_hpp
#define leaf_printer_hpp

#include "expr.hpp"
#include <vector>

struct Printer : public ExprVisitor {
    virtual auto execute(std::vector<Expr*>& stmts) -> void override;
    virtual auto visit_ternaryexpr(TernaryExpr* expr) -> Object* override;
    virtual auto visit_binaryexpr(BinaryExpr* expr) -> Object* override;
    virtual auto visit_unaryexpr(UnaryExpr* expr) -> Object* override;
    virtual auto visit_exponentexpr(ExponentExpr* expr) -> Object* override;
    virtual auto visit_literalexpr(LiteralExpr* expr) -> Object* override;
    virtual auto visit_identifierexpr(IdentifierExpr* expr) -> Object* override;
    virtual auto visit_groupingexpr(GroupingExpr* expr) -> Object* override;
    virtual auto visit_nilexpr(NilExpr* expr) -> Object* override;

private:
    auto evaluate(Expr* expr) -> Object*;
};

#endif
