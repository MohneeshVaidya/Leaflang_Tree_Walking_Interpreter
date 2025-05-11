#include "expr.hpp"
#include "leaf_object.hpp"
#include "expr_printer.hpp"

#include <format>
#include <iostream>


// Private methods
auto ExprPrinter::evaluate(Expr* expr) -> LeafObject* {
    return expr->accept(this);
}


// Public methods
auto ExprPrinter::execute(std::vector<Expr*>& expressions) -> LeafObject* {
    for (Expr* expr : expressions) {
        if (expr->type() != ExprType::k_null) {
            this->evaluate(expr);
            std::cout << "\n";
        }
    }
    return nullptr;
}

auto ExprPrinter::visit_nullexpr([[maybe_unused]] NullExpr* expr) -> LeafObject* {
    std::cout << "() ";
    return nullptr;
}

auto ExprPrinter::visit_ternaryexpr([[maybe_unused]] TernaryExpr* expr) -> LeafObject* {
    std::cout << "( ";
    evaluate(expr->condition);
    std::cout << "? ";
    evaluate(expr->first);
    std::cout << ": ";
    evaluate(expr->second);
    std::cout << ") ";
    return nullptr;
}

auto ExprPrinter::visit_binaryexpr(BinaryExpr* expr) -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}

auto ExprPrinter::visit_unaryexpr(UnaryExpr* expr) -> LeafObject* {
    std::cout << std::format("( {} ", expr->oper->lexeme());
    evaluate(expr->operand);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_exponentexpr(ExponentExpr* expr) -> LeafObject* {
    std::cout << "( ** ";
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_groupingexpr(GroupingExpr* expr) -> LeafObject* {
    std::cout << "( ";
    evaluate(expr->expression);
    std::cout << ") ";
    return nullptr;
}
auto ExprPrinter::visit_primaryexpr(PrimaryExpr* expr) -> LeafObject* {
    std::cout << expr->token->lexeme() << " ";
    return nullptr;
}
