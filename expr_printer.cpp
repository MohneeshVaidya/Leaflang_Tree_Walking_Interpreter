#include "expr.hpp"
#include "leaf_object.hpp"
#include "expr_printer.hpp"

#include <format>
#include <iostream>


// Private methods
auto ExprPrinter::evaluate(const Expr* expr) const -> LeafObject* {
    return expr->accept(this);
}


// Public methods
auto ExprPrinter::execute(const std::vector<const Expr*>& expressions) const -> LeafObject* {
    for (const Expr* expr : expressions) {
        if (expr->type() != ExprType::k_null) {
            this->evaluate(expr);
            std::cout << "\n";
        }
    }
    return nullptr;
}

auto ExprPrinter::visit_nullexpr([[maybe_unused]] const NullExpr* expr) const -> LeafObject* {
    std::cout << "() ";
    return nullptr;
}

auto ExprPrinter::visit_ternaryexpr([[maybe_unused]] const TernaryExpr* expr) const -> LeafObject* {
    std::cout << "( ";
    evaluate(expr->condition);
    std::cout << "? ";
    evaluate(expr->first);
    std::cout << ": ";
    evaluate(expr->second);
    std::cout << ") ";
    return nullptr;
}

auto ExprPrinter::visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}

auto ExprPrinter::visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->operand);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* {
    std::cout << "( ** ";
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* {
    std::cout << "( ";
    evaluate(expr->expression);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* {
    std::cout << expr->token->lexeme() << " ";
    return nullptr;
}
