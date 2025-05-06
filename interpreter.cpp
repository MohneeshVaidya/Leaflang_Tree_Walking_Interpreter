#include "interpreter.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"
#include "token_type.hpp"
#include "tools/binary_operations.hpp"

#include <cstdlib>

using namespace std::string_literals;

using enum TokenType;

// Private methods
auto Interpreter::evaluate(const Expr* expr) const -> LeafObject* {
    return expr->accept(this);
}

// Public methods
auto Interpreter::execute(const std::vector<const Expr*>& expressions) const -> LeafObject* {
    for (const Expr* expr : expressions) {
        if (expr->type() != ExprType::k_null) {
            std::cout << *evaluate(expr) << "\n";
        }
    }
    return nullptr;
}

auto Interpreter::visit_nullexpr([[maybe_unused]] const NullExpr* expr) const -> LeafObject* {
    return nullptr;
}

auto Interpreter::visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* {
    const LeafObject* condition { evaluate(expr->condition) };
    if (condition->is_truthy()) {
        return evaluate(expr->first);
    }
    return evaluate(expr->second);
}

auto Interpreter::visit_binaryexpr([[maybe_unused]] const BinaryExpr* expr) const -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_unaryexpr([[maybe_unused]] const UnaryExpr* expr) const -> LeafObject* {
    auto operand { evaluate(expr->operand) };
    if (expr->oper->lexeme() == "-") {
        if (operand->type() != ObjectType::k_number) {
            LeafError::instance()->runtime_error(expr->oper->line(), expr->oper->lexeme(), "Operand must be a number."s);
        }
        return LeafNumber::create_object(-dynamic_cast<const LeafNumber*>(operand)->value());
    } else {
        return LeafBool::create_object( !operand->is_truthy() );
    }
}

auto Interpreter::visit_exponentexpr([[maybe_unused]] const ExponentExpr* expr) const -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_groupingexpr([[maybe_unused]] const GroupingExpr* expr) const -> LeafObject* {
    return evaluate(expr->expression);
}

auto Interpreter::visit_primaryexpr([[maybe_unused]] const PrimaryExpr* expr) const -> LeafObject* {
    if (expr == nullptr) {
        return nullptr;
    }

    auto type { expr->token->type() };

    if (type == k_number) {
        return LeafNumber::create_object(expr->token);
    } else if (type == k_string) {
        return LeafString::create_object(expr->token);
    } else if (type == k_true) {
        return LeafBool::create_object(expr->token);
    } else if (type == k_false) {
        return LeafBool::create_object(expr->token);
    } else if (type == k_null) {
        return LeafNull::create_object(expr->token);
    }
    return nullptr;
}
