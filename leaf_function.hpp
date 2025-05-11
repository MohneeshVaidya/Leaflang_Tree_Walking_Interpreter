#ifndef LEAF_FUNCTION_HPP
#define LEAF_FUNCTION_HPP

#include "environment.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "leaf_object.hpp"
#include "stmt.hpp"
#include "token.hpp"

#include <vector>

// FunctionExpr
class FunctionExpr : public Expr {
public:
    Token* identifier;
    std::vector<Token*> parameters;
    Stmt* block_stmt;

private:
    FunctionExpr(Token* identifier, std::vector<Token*>& parameters, Stmt* block_stmt);
    FunctionExpr(FunctionExpr& source) = default;
    auto operator = (FunctionExpr&) -> FunctionExpr& { return *this; }

public:
    ~FunctionExpr();
    static auto create_object(Token* identifier, std::vector<Token*>& parameters, Stmt* block_stmt) -> FunctionExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// CallExpr
class CallExpr : public Expr {
public:
    Token* identifier;
    Expr* expr;   // Either function or call expression
    std::vector<Expr*> arguments;

private:
    CallExpr(Token* identifier, std::vector<Expr*>& arguments);
    CallExpr(Expr* expr, std::vector<Expr*>& arguments);
    CallExpr(CallExpr& source) = default;
    auto operator = (CallExpr& other) -> CallExpr& = default;

public:
    ~CallExpr();
    static auto create_object(Token* identifier, std::vector<Expr*>& arguments) -> CallExpr*;
    static auto create_object(Expr* expr, std::vector<Expr*>& arguments) -> CallExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// LeafFunction
class LeafFunction : public LeafObject {
public:
    std::vector<Token*> parameters;
    Stmt* block_stmt;
    Environment* closure;

private:
    LeafFunction(std::vector<Token*>& parameters, Stmt* block_stmt, Environment* closure);
    LeafFunction(LeafFunction& source) = default;
    auto operator = (LeafFunction&) -> LeafFunction&;

public:
    ~LeafFunction();
    static auto create_object(std::vector<Token*>& parameters, Stmt* block_stmt, Environment* closure) -> LeafFunction*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto call(std::vector<Expr*>& arguments, Interpreter* interpreter) -> LeafObject*;
};

#endif
