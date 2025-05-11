#include "interpreter.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"
#include "leaf_function.hpp"
#include "leaf_struct.hpp"
#include "stmt.hpp"
#include "token.hpp"
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
auto Interpreter::execute_stmt(Stmt* stmt) -> void {
    stmt->accept(this);
}

auto Interpreter::evaluate(Expr* expr) -> LeafObject* {
    return expr->accept(this);
}


// Public methods
auto Interpreter::execute(std::vector<Stmt*>& statements) -> void {
    for (Stmt* stmt : statements) {
        execute_stmt(stmt);
    }
}

auto Interpreter::execute(std::vector<Expr*>& expressions) -> LeafObject* {
    for (Expr* expr : expressions) {
        if (expr->type() != ExprType::k_null) {
            std::cout << evaluate(expr) << "\n";
        }
    }
    return nullptr;
}


auto Interpreter::visit_printstmt(PrintStmt* stmt) -> void {
    LeafObject* value { evaluate(stmt->expr) };
    std::cout << value;
}

auto Interpreter::visit_printlnstmt(PrintlnStmt* stmt) -> void {
    LeafObject* value { evaluate(stmt->expr) };
    std::cout << value << "\n";
}

auto Interpreter::visit_expressionstmt(ExpressionStmt* stmt) -> void {
    evaluate(stmt->expr);
}

auto Interpreter::visit_varstmt(VarStmt* stmt) -> void {
    m_environment->insert_var(stmt->identifier, evaluate(stmt->expr));
}

auto Interpreter::visit_conststmt(ConstStmt* stmt) -> void {
    m_environment->insert_const(stmt->identifier, evaluate(stmt->expr));
}

auto Interpreter::visit_blockstmt(BlockStmt* stmt, Environment* environment) -> void {
    environment->set_parent(m_environment);
    m_environment = environment;

    try {
        for (Stmt* statement : stmt->statements) {
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

auto Interpreter::visit_ifstmt(IfStmt* stmt) -> void {
    for (auto statement : stmt->statements) {
        if (statement.first == nullptr) {
            execute_stmt(statement.second);
            break;
        }
        LeafObject* condition { evaluate(statement.first) };
        if (condition->is_truthy()) {
            execute_stmt(statement.second);
            break;
        }
    }
}

auto Interpreter::visit_forstmt(ForStmt* stmt) -> void {
    BlockCtx prev_block_ctx { m_block_ctx };
    m_block_ctx = BlockCtx::k_loop;
    try {
        if (stmt->condition == nullptr) {
            while (true) {
                try {
                    execute_stmt(stmt->block_stmt);
                } catch (Continue&) {
                }
            }
            return;
        }

        while (evaluate(stmt->condition)->is_truthy()) {
            try {
                execute_stmt(stmt->block_stmt);
            } catch (Continue&) {
            }
        }
    } catch (Break&) {
    }
    m_block_ctx = prev_block_ctx;
}

auto Interpreter::visit_breakstmt(BreakStmt* stmt) -> void {
    if (m_block_ctx == BlockCtx::k_loop) {
        throw Break { };
        return;
    }
    LeafError::instance()->runtime_error(
        stmt->line,
        "'break' can only be used within the context of a loop."s
    );
}

auto Interpreter::visit_continuestmt(ContinueStmt* stmt) -> void {
    if (m_block_ctx == BlockCtx::k_loop) {
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

auto Interpreter::visit_returnstmt(ReturnStmt* stmt) -> void {
    if (m_called_ctx == CalledCtx::k_function) {
        throw evaluate(stmt->value);
    }
    if (m_called_ctx == CalledCtx::k_constructor) {
        LeafError::instance()->runtime_error(
            stmt->token->line(), "'return' statement can not be used inside constructor."s);
    }
    LeafError::instance()->runtime_error(
        stmt->token->line(), "'return' statement can only be used within the context of a function."s);
}


auto Interpreter::visit_nullexpr(NullExpr*) -> LeafObject* {
    return LeafNull::create_object();
}

auto Interpreter::visit_assignexpr(AssignExpr* expr) -> LeafObject* {
    Token* left { expr->left };
    if (left->type() != k_identifier) {
        LeafError::instance()->runtime_error(expr->oper->line(), expr->oper->lexeme(), "Left hand operator of '=' must be a variable."s);
    }
    LeafObject* right { evaluate(expr->right) };

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

auto Interpreter::visit_ternaryexpr(TernaryExpr* expr) -> LeafObject* {
    LeafObject* condition { evaluate(expr->condition) };
    if (condition->is_truthy()) {
        return evaluate(expr->first);
    }
    return evaluate(expr->second);
}

auto Interpreter::visit_binaryexpr(BinaryExpr* expr) -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_unaryexpr(UnaryExpr* expr) -> LeafObject* {
    auto operand { evaluate(expr->operand) };
    if (expr->oper->lexeme() == "-") {
        if (operand->type() != ObjectType::k_number) {
            LeafError::instance()->runtime_error(expr->oper->line(), expr->oper->lexeme(), "Operand must be a number."s);
        }
        return LeafNumber::create_object(-dynamic_cast<LeafNumber*>(operand)->value());
    } else {
        return LeafBool::create_object( !operand->is_truthy() );
    }
}

auto Interpreter::visit_exponentexpr(ExponentExpr* expr) -> LeafObject* {
    auto left { evaluate(expr->left) };
    auto right { evaluate(expr->right) };
    return BinaryOperations::instance()->execute(expr->oper, left, right);
}

auto Interpreter::visit_groupingexpr(GroupingExpr* expr) -> LeafObject* {
    return evaluate(expr->expression);
}

auto Interpreter::visit_primaryexpr(PrimaryExpr* expr) -> LeafObject* {
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

auto Interpreter::visit_functionexpr(FunctionExpr* expr) -> LeafObject* {
    return LeafFunction::create_object(expr->parameters, expr->block_stmt, m_environment->make_closure());
}

auto Interpreter::visit_callexpr(CallExpr* expr) -> LeafObject* {
    LeafObject* value { };

    if (expr->identifier) {
        if (m_is_method) {
            value = m_current_instance->value().at(expr->identifier->lexeme());
        } else {
            value = m_environment->get(expr->identifier);
        }
        if (value->type() != ObjectType::k_function && value->type() != ObjectType::k_struct) {
            LeafError::instance()->runtime_error(
                expr->identifier->line(),
                std::format("'{}' is not callable.", expr->identifier->lexeme()));
        }
    } else if (expr->expr) {
        value = evaluate(expr->expr);
    }

    CalledCtx prev_ctx { m_called_ctx };
    LeafStructInstance* previous_instance { m_current_instance };

    LeafFunction* casted_value { };
    if (value->type() == ObjectType::k_struct) {
        m_called_ctx = CalledCtx::k_constructor;

        casted_value = m_environment->metaclass_get_method(expr->identifier, "__construct"s);
        if (casted_value == nullptr) {
            LeafError::instance()->runtime_error(
                expr->identifier->line(),
                std::format("No constructor is provided for struct '{}'", expr->identifier->lexeme())
            );
        }

        LeafStruct* leaf_struct { dynamic_cast<LeafStruct*>(value) };

        std::unordered_map<std::string, LeafObject*> fields { };
        for (Token* field : leaf_struct->value()) {
            fields.insert_or_assign(field->lexeme(), LeafNull::create_object());
        }
        m_current_instance = LeafStructInstance::create_object(leaf_struct->identifier, fields);

    } else if (value->type() == ObjectType::k_function) {
        m_called_ctx = CalledCtx::k_function;
        casted_value = dynamic_cast<LeafFunction*>(value);
    }

    if (casted_value->parameters.size() != expr->arguments.size()) {
        LeafError::instance()->runtime_error(
            expr->identifier->line(),
            std::format("Number of arguments does not match number of parameters for function '{}'.", expr->identifier->lexeme()));
    }

    LeafObject* return_value { casted_value->call(expr->arguments, this) };

    m_called_ctx = prev_ctx;
    m_current_instance = previous_instance;
    return return_value;
}

auto Interpreter::visit_structexpr(StructExpr* expr) -> LeafObject* {
    LeafStruct* leaf_struct { LeafStruct::create_object(expr->identifier, expr->fields) };
    m_environment->insert_const(expr->identifier, leaf_struct);

    for (FunctionExpr* func_expr : expr->methods) {
        m_environment->metaclass_add_method(expr->identifier, func_expr->identifier->lexeme(), dynamic_cast<LeafFunction*>(evaluate(func_expr)));
    }
    return leaf_struct;
}

auto Interpreter::visit_getexpr(GetExpr* expr) -> LeafObject* {
    if (expr->left) {
        LeafObject* value { m_environment->get(expr->left) };
        return value;
    }

    LeafObject* left { evaluate(expr->left_expr) };

    std::string member { };
    if (expr->right_expr == nullptr) {
        member.assign("Field");
    } else {
        member.assign("Method");
    }
    if (left->type() != ObjectType::k_struct_instance) {
        LeafError::instance()->runtime_error(
            expr->right->line(),
            std::format("{} '{}' can not be called on a non-struct value.", member, expr->right->lexeme())
        );
    }
    LeafStructInstance* instance { dynamic_cast<LeafStructInstance*>(left) };
    if (instance->value().contains(expr->right->lexeme()) == false) {
        LeafError::instance()->runtime_error(
            expr->right->line(),
            std::format("{} '{}' does not exist on given struct value.", member, expr->right->lexeme()));
    }

    if (expr->right_expr == nullptr) {
        return instance->value().at(expr->right->lexeme());
    }

    auto is_method_snap { m_is_method };
    m_is_method = true;

    auto current_instance_snap { m_current_instance };
    m_current_instance = instance;

    LeafObject* result { evaluate(expr->right_expr) };

    m_current_instance = current_instance_snap;
    m_is_method = is_method_snap;
    return result;
}

auto Interpreter::visit_setexpr([[maybe_unused]] SetExpr* expr) -> LeafObject* {
    LeafObject* value { evaluate(expr->left->left_expr) };
    if (value->type() != ObjectType::k_struct_instance) {
        LeafError::instance()->runtime_error(
            expr->left->right->line(),
            std::format("Field '{}' can not be called on non-struct value.", expr->left->right->lexeme()));
    }
    LeafStructInstance* instance { dynamic_cast<LeafStructInstance*>(value) };
    if (instance->value().contains(expr->left->right->lexeme()) == false) {
        LeafError::instance()->runtime_error(
            expr->left->right->line(),
            std::format("Field '{}' does not exist on given struct value.", expr->left->right->lexeme()));
    }
    LeafObject* value_to_assign { evaluate(expr->right) };
    instance->value().insert_or_assign(
        expr->left->right->lexeme(), value_to_assign);
    return value_to_assign;
}
