#include "printer.hpp"

#include <iostream>
#include <vector>

auto Printer::execute(std::vector<Expr*>& stmts) -> void {
    for (Expr* expr : stmts) {
        evaluate(expr);
        std::cout << "\n";
    }
}


// private methdos
auto Printer::evaluate(Expr* expr) -> Object* {
    return expr->accept(this);
}


// protected methods
auto Printer::visit_ternaryexpr(TernaryExpr* expr) -> Object* {
    std::cout << "( ";
    evaluate(expr->condition);
    std::cout << "? ";
    evaluate(expr->first);
    std::cout << ": ";
    evaluate(expr->second);
    std::cout << ") ";
    return nullptr;
}

auto Printer::visit_binaryexpr(BinaryExpr* expr) -> Object* {
    std::cout << "( " << expr->oper->lexeme << " ";
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}

auto Printer::visit_unaryexpr(UnaryExpr* expr) -> Object* {
    std::cout << "( " << expr->oper->lexeme << " ";
    evaluate(expr->expr);
    std::cout << ") ";
    return nullptr;
}

auto Printer::visit_exponentexpr(ExponentExpr* expr) -> Object* {
    std::cout << "( ** ";
    evaluate(expr->left);
    evaluate(expr->right);
    std::cout << ") ";
    return nullptr;
}

auto Printer::visit_literalexpr(LiteralExpr* expr) -> Object* {
    std::cout << expr->value->lexeme << " ";
    return nullptr;
}

auto Printer::visit_identifierexpr(IdentifierExpr* expr) -> Object* {
    std::cout << expr->name->lexeme << " ";
    return nullptr;
}

auto Printer::visit_groupingexpr(GroupingExpr* expr) -> Object* {
    std::cout << "( ";
    evaluate(expr->expr);
    std::cout << ") ";
    return nullptr;
}

auto Printer::visit_nilexpr([[maybe_unused]] NilExpr* expr) -> Object* {
    std::cout << "nil ";
    return nullptr;
}
