#include "interpreter.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"
#include "leaf_function.hpp"
#include "stmt.hpp"
#include "token_type.hpp"
#include "tools/binary_operations.hpp"

#include <cstdlib>
#include <format>

using namespace std::string_literals;

using enum TokenType;


// Break
class Break { };

// Continue
class Continue { };


// Interpreter
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

auto Interpreter::visit_blockstmt(const BlockStmt* stmt, Environment* environment) -> void {
    environment->set_parent(m_environment);
    m_environment = environment;

    try {
        for (const Stmt* statement : stmt->statements) {
            execute_stmt(statement);
        }
    } catch (LeafObject* return_value) {
        m_environment = environment->parent();
        Environment::delete_object(environment);
        throw return_value;
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

auto Interpreter::visit_forstmt(const ForStmt* stmt) -> void {
    const BlockCtx prev_block_ctx { block_ctx };
    block_ctx = BlockCtx::k_loop;
    try {
        if (stmt->condition == nullptr) {
            while (true) {
                try {
                    execute_stmt(stmt->block_stmt);
                } catch (const Continue&) {
                }
            }
            return;
        }

        while (evaluate(stmt->condition)->is_truthy()) {
            try {
                execute_stmt(stmt->block_stmt);
            } catch (const Continue&) {
            }
        }
    } catch (const Break&) {
    }
    block_ctx = prev_block_ctx;
}

auto Interpreter::visit_breakstmt(const BreakStmt* stmt) const -> void {
    if (block_ctx == BlockCtx::k_loop) {
        throw Break { };
        return;
    }
    LeafError::instance()->runtime_error(
        stmt->line,
        "'break' can only be used within the context of a loop."s
    );
}

auto Interpreter::visit_continuestmt(const ContinueStmt* stmt) const -> void {
    if (block_ctx == BlockCtx::k_loop) {
        if (stmt->step_expr) {
            evaluate(stmt->step_expr);
        }
        throw Continue { };
        return;
    }
    LeafError::instance()->runtime_error(
        stmt->line,
        "'continue' can only be used within the context of a loop."s
    );
}

auto Interpreter::visit_returnstmt(const ReturnStmt* stmt) const -> void {
    if (func_ctx == FunctionCtx::k_function) {
        throw evaluate(stmt->value);
    }
    LeafError::instance()->runtime_error(
        stmt->token->line(), "'return' statement can only be used within the context of a function."s);
}


auto Interpreter::visit_nullexpr(const NullExpr*) const -> LeafObject* {
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

auto Interpreter::visit_functionexpr(const FunctionExpr* expr) const -> LeafObject* {
    return LeafFunction::create_object(expr->parameters, expr->block_stmt, m_environment->make_closure());
}

auto Interpreter::visit_callexpr(const CallExpr* expr) -> LeafObject* {
    const FunctionCtx prev_ctx { func_ctx };
    func_ctx = FunctionCtx::k_function;

    const LeafObject* value { };

    if (expr->identifier) {
        value = m_environment->get(expr->identifier);
        if (value->type() != ObjectType::k_function) {
            LeafError::instance()->runtime_error(
                expr->identifier->line(),
                std::format("'{}' is not callable.", expr->identifier->lexeme()));
        }
    } else if (expr->expr) {
        value = evaluate(expr->expr);
    }

    LeafFunction* casted_value { const_cast<LeafFunction*>(dynamic_cast<const LeafFunction*>(value)) };

    if (casted_value->parameters.size() != expr->arguments.size()) {
        LeafError::instance()->runtime_error(
            expr->identifier->line(),
            std::format("Number of arguments does not match number of parameters for function '{}'.", expr->identifier->lexeme()));
    }

    LeafObject* return_value { casted_value->call(expr->arguments, const_cast<Interpreter*>(this)) };

    func_ctx = prev_ctx;
    return return_value;
}
