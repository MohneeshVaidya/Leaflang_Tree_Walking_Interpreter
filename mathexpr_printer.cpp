#include "expr.hpp"
#include "leaf_object.hpp"
#include "mathexpr_printer.hpp"

#include <format>
#include <iostream>


// Private methods
auto MathExprPrinter::evaluate(const Expr* expr) const -> LeafObject* {
    return expr->accept(this);
}


// Public methods
auto MathExprPrinter::print(const std::vector<const Expr*>& expressions) const -> void {
    for (const Expr* expr : expressions) {
        if (expr != nullptr) {
            this->evaluate(expr);
            std::cout << "\n";
        }
    }
}

auto MathExprPrinter::visit_ternaryexpr([[maybe_unused]] const TernaryExpr* expr) const -> LeafObject* {
    return nullptr;
}

auto MathExprPrinter::visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}

auto MathExprPrinter::visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->operand);
    std::cout << ") ";
    return nullptr;
}
auto MathExprPrinter::visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* {
    std::cout << "( ** ";
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}
auto MathExprPrinter::visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* {
    std::cout << "( ";
    evaluate(expr->expression);
    std::cout << ") ";
    return nullptr;
}
auto MathExprPrinter::visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* {
    std::cout << expr->token->lexeme() << " ";
    return nullptr;
}
