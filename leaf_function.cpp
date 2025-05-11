#include "leaf_function.hpp"
#include "environment.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"

#include <cstdint>

// FunctionExpr
FunctionExpr::FunctionExpr(Token* identifier, std::vector<Token*>& parameters, Stmt* block_stmt) :
    identifier { identifier },
    parameters { parameters },
    block_stmt { block_stmt }
    {
    }

FunctionExpr::~FunctionExpr() {
    Stmt::delete_object(block_stmt);
}

auto FunctionExpr::create_object(Token* identifier, std::vector<Token*>& parameters, Stmt* block_stmt) -> FunctionExpr* {
    return new FunctionExpr { identifier, parameters, block_stmt };
}

auto FunctionExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_functionexpr(this);
}

auto FunctionExpr::type() -> ExprType {
    return ExprType::k_function;
}


// CallExpr
CallExpr::CallExpr(Token* identifier, std::vector<Expr*>& arguments) :
    identifier { identifier },
    expr { nullptr },
    arguments { arguments }
    {
    }

CallExpr::CallExpr(Expr* expr, std::vector<Expr*>& arguments) :
    identifier { nullptr },
    expr { expr },
    arguments { arguments }
    {
    }

CallExpr::~CallExpr() {
    Expr::delete_object(expr);
    for (Expr* expr : arguments) {
        Expr::delete_object(expr);
    }
}

auto CallExpr::create_object(Token* identifier, std::vector<Expr*>& arguments) -> CallExpr* {
    return new CallExpr { identifier, arguments };
}

auto CallExpr::create_object(Expr* expr, std::vector<Expr*>& arguments) -> CallExpr* {
    return new CallExpr { expr, arguments };
}

auto CallExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return const_cast<ExprVisitor*>(visitor)->visit_callexpr(this);
}

auto CallExpr::type() -> ExprType {
    return ExprType::k_call;
}


// LeafFunction
LeafFunction::LeafFunction(std::vector<Token*>& parameters, Stmt* block_stmt, Environment* closure) :
    LeafObject { nullptr },
    parameters { parameters},
    block_stmt { block_stmt },
    closure { closure }
    {
    }

LeafFunction::~LeafFunction() {
    delete closure;
}

auto LeafFunction::create_object(std::vector<Token*>& parameters, Stmt* block_stmt, Environment* closure) -> LeafFunction* {
    return new LeafFunction { parameters, block_stmt, closure };
}

auto LeafFunction::type() -> ObjectType {
    return ObjectType::k_function;
}

auto LeafFunction::is_truthy() -> bool {
    return true;
}

auto LeafFunction::call(std::vector<Expr*>& arguments, Interpreter* interpreter) -> LeafObject* {
    Environment* environment { Environment::create_object() };

    for (uint32_t i { 0 }; i < parameters.size(); i++) {
        environment->insert_var(
            parameters.at(i), interpreter->evaluate(arguments.at(i)));
    }

    Environment* previous_envioronment { interpreter->m_environment };
    interpreter->m_environment = closure;

    try {
        interpreter->visit_blockstmt(dynamic_cast<BlockStmt*>(block_stmt), environment);
        interpreter->m_environment = previous_envioronment;
        if (interpreter->m_called_ctx == CalledCtx::k_constructor) {
            return interpreter->m_current_instance;
        }
        return LeafNull::create_object();
    } catch (LeafObject* return_value) {
        interpreter->m_environment = previous_envioronment;
        return return_value;
    }
}
