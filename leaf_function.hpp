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
    std::vector<const Token*> parameters;
    const Stmt* block_stmt;

private:
    FunctionExpr(const std::vector<const Token*>& parameters, const Stmt* block_stmt);
    FunctionExpr(const FunctionExpr& source) = default;
    auto operator = (const FunctionExpr&) -> FunctionExpr& { return *this; }

public:
    ~FunctionExpr();

    static auto create_object(const std::vector<const Token*>& parameters, const Stmt* block_stmt) -> FunctionExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};


// CallExpr
class CallExpr : public Expr {
public:
    const Token* identifier;
    const Expr* expr;   // Either function or call expression
    std::vector<const Expr*> arguments;

private:
    CallExpr(const Token* identifier, const std::vector<const Expr*>& arguments);
    CallExpr(const Expr* expr, const std::vector<const Expr*>& arguments);
    CallExpr(const CallExpr& source) = default;
    auto operator = (const CallExpr& other) -> CallExpr& = default;

public:
    ~CallExpr();
    static auto create_object(const Token* identifier, const std::vector<const Expr*>& arguments) -> CallExpr*;
    static auto create_object(const Expr* expr, const std::vector<const Expr*>& arguments) -> CallExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};


// LeafFunction
class LeafFunction : public LeafObject {
public:
    std::vector<const Token*> parameters;
    const Stmt* block_stmt;
    Environment* closure;

private:
    LeafFunction(const std::vector<const Token*>& parameters, const Stmt* block_stmt, Environment* closure);
    LeafFunction(const LeafFunction& source) = default;
    auto operator = (const LeafFunction&) -> LeafFunction&;

public:
    ~LeafFunction();
    static auto create_object(const std::vector<const Token*>& parameters, const Stmt* block_stmt, Environment* closure) -> LeafFunction*;

    virtual auto type() const -> ObjectType override;
    virtual auto is_truthy() const -> bool override;

    auto call(const std::vector<const Expr*>& arguments, Interpreter* interpreter) -> LeafObject*;
};

#endif
