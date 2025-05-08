#include "interpreter.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"
#include "token_type.hpp"
#include "tools/binary_operations.hpp"

#include <cstdlib>
#include <format>

using namespace std::string_literals;

using enum TokenType;


Interpreter::Interpreter(Environment* environment) :
    m_environment { environment }
    {
    }

Interpreter::~Interpreter() {
    Environment::delete_object(m_environment);
}


// Private methods
auto Interpreter::execute_stmt(const Stmt* stmt) const -> void {
    stmt->accept(this);
}

auto Interpreter::evaluate(const Expr* expr) const -> LeafObject* {
    return expr->accept(this);
}


// Public methods
auto Interpreter::execute(const std::vector<const Stmt*>& statements) const -> void {
    for (const Stmt* stmt : statements) {
        execute_stmt(stmt);
    }
}

auto Interpreter::execute(const std::vector<const Expr*>& expressions) const -> LeafObject* {
    for (const Expr* expr : expressions) {
        if (expr->type() != ExprType::k_null) {
            std::cout << evaluate(expr) << "\n";
        }
    }
    return nullptr;
}


auto Interpreter::visit_printstmt(const PrintStmt* stmt) const -> void {
    const LeafObject* value { evaluate(stmt->expr) };
    std::cout << value;
}

auto Interpreter::visit_printlnstmt(const PrintlnStmt* stmt) const -> void {
    const LeafObject* value { evaluate(stmt->expr) };
    std::cout << value << "\n";
}

auto Interpreter::visit_expressionstmt(const ExpressionStmt* stmt) const -> void {
    evaluate(stmt->expr);
}

auto Interpreter::visit_varstmt(const VarStmt* stmt) const -> void {
    m_environment->insert_var(stmt->identifier, evaluate(stmt->expr));
}

auto Interpreter::visit_conststmt(const ConstStmt* stmt) const -> void {
    m_environment->insert_const(stmt->identifier, evaluate(stmt->expr));
}

auto Interpreter::visit_blockstmt(const BlockStmt* stmt) -> void {
    Environment* environment { Environment::create_object(m_environment) };
    m_environment = environment;

    for (const Stmt* statement : stmt->statements) {
        execute_stmt(statement);
    }

    m_environment = environment->parent();
    Environment::delete_object(environment);
}

auto Interpreter::visit_ifstmt(const IfStmt* stmt) const -> void {
    for (auto statement : stmt->statements) {
        if (statement.first == nullptr) {
            execute_stmt(statement.second);
            break;
        }
        const LeafObject* condition { evaluate(statement.first) };
        if (condition->is_truthy()) {
            execute_stmt(statement.second);
            break;
        }
    }
}

auto Interpreter::visit_forstmt(const ForStmt* stmt) const -> void {
    if (stmt->condition == nullptr) {
        while (true) {
            execute_stmt(stmt->block_stmt);
        }
        return;
    }

    while (evaluate(stmt->condition)->is_truthy()) {
        execute_stmt(stmt->block_stmt);
    }
}


auto Interpreter::visit_nullexpr([[maybe_unused]] const NullExpr* expr) const -> LeafObject* {
    return LeafNull::create_object();
}

auto Interpreter::visit_assignexpr(const AssignExpr* expr) const -> LeafObject* {
    const Token* left { expr->left };
    if (left->type() != k_identifier) {
        LeafError::instance()->runtime_error(expr->oper->line(), expr->oper->lexeme(), "Left hand operator of '=' must be a variable."s);
    }
    const LeafObject* right { evaluate(expr->right) };

    if (m_environment->get_qualifier(left) == "const"s) {
        LeafError::instance()->runtime_error(
            expr->oper->line(),
            expr->oper->lexeme(),
            std::format("Const variable '{}' can not be re-assigned a value.", left->lexeme())
        );
    }

    m_environment->assign(left, right);
    return const_cast<LeafObject*>(right);
}

auto Interpreter::visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* {
    const LeafObject* condition { evaluate(expr->condition) };
    if (condition->is_truthy()) {
        return evaluate(expr->first);
    }
    return evaluate(expr->second);
}

auto Interpreter::visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* {
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

auto Interpreter::visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* {
    return evaluate(expr->expression);
}

auto Interpreter::visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* {
    if (expr == nullptr) {
        return nullptr;
    }

    auto type { expr->token->type() };

    if (type == k_number) {
        return LeafNumber::create_object(expr->token);
    } else if (type == k_string) {
        return LeafString::create_object(expr->token);
    } else if (type == k_true || type == k_false) {
        return LeafBool::create_object(expr->token);
    } else if (type == k_null) {
        return LeafNull::create_object(expr->token);
    } else if (type == k_identifier) {
        return const_cast<LeafObject*>(m_environment->get(expr->token));
    }
    return nullptr;
}
