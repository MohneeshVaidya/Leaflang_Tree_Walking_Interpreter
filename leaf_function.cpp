#include "leaf_function.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"

#include <cstdint>

// FunctionExpr
FunctionExpr::FunctionExpr(const std::vector<const Token*>& parameters, const Stmt* block_stmt) :
    parameters { parameters },
    block_stmt { block_stmt }
    {
    }

FunctionExpr::~FunctionExpr() {
    Stmt::delete_object(block_stmt);
}

auto FunctionExpr::create_object(const std::vector<const Token*>& parameters, const Stmt* block_stmt) -> FunctionExpr* {
    return new FunctionExpr { parameters, block_stmt };
}

auto FunctionExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_functionexpr(this);
}

auto FunctionExpr::type() const -> ExprType {
    return ExprType::k_function;
}


// CallExpr
CallExpr::CallExpr(const Token* identifier, const std::vector<const Expr*>& arguments) :
    identifier { identifier },
    arguments { arguments }
    {
    }

CallExpr::~CallExpr() {
    for (const Expr* expr : arguments) {
        Expr::delete_object(expr);
    }
}

auto CallExpr::create_object(const Token* identifier, const std::vector<const Expr*>& arguments) -> CallExpr* {
    return new CallExpr { identifier, arguments };
}

auto CallExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return const_cast<ExprVisitor*>(visitor)->visit_callexpr(this);
}

auto CallExpr::type() const -> ExprType {
    return ExprType::k_call;
}


// LeafFunction
LeafFunction::LeafFunction(const std::vector<const Token*>& parameters, const Stmt* block_stmt, Environment* closure) :
    LeafObject { nullptr },
    parameters { parameters},
    block_stmt { block_stmt },
    closure { closure }
    {
    }

LeafFunction::~LeafFunction() {
    delete closure;
}

auto LeafFunction::create_object(const std::vector<const Token*>& parameters, const Stmt* block_stmt, Environment* closure) -> LeafFunction* {
    return new LeafFunction { parameters, block_stmt, closure };
}

auto LeafFunction::type() const -> ObjectType {
    return ObjectType::k_function;
}

auto LeafFunction::is_truthy() const -> bool {
    return true;
}

auto LeafFunction::call(const std::vector<const Expr*>& arguments, Interpreter* interpreter) -> LeafObject* {
    Environment* environment { Environment::create_object() };

    for (uint32_t i { 0 }; i < parameters.size(); i++) {
        environment->insert_var(
            parameters.at(i), interpreter->evaluate(arguments.at(i)));
    }

    Environment* environment_snapshot { interpreter->m_environment };
    interpreter->m_environment = closure;

    try {
        interpreter->visit_blockstmt(dynamic_cast<const BlockStmt*>(block_stmt), environment);
        interpreter->m_environment = environment_snapshot;
        return LeafNull::create_object();
    } catch (LeafObject* return_value) {
        interpreter->m_environment = environment_snapshot;
        return return_value;
    }
}
