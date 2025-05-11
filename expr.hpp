#ifndef EXPR_HPP
#define EXPR_HPP

#include "leaf_object.hpp"
#include "token.hpp"
#include <vector>


// ExprType enum
enum class ExprType {
    k_null,
    k_assign,
    k_ternary,
    k_binary,
    k_unary,
    k_exponent,
    k_grouping,
    k_primary,
    k_function,
    k_call,
    k_struct,
    k_get,
    k_set,
};


// ExprVisitor forward declaration
class ExprVisitor;

// Expr classes
class Expr {
public:
    static auto delete_object(Expr* object) -> void;

    virtual ~Expr() = default;
    virtual auto accept(ExprVisitor* visitor) -> LeafObject* = 0;
    virtual auto type() -> ExprType = 0;
};

// NullExpr
class NullExpr: public Expr {
private:
    NullExpr() = default;
    NullExpr(NullExpr& source) = default;
    auto operator = (NullExpr& other) -> NullExpr& = default;

public:
    ~NullExpr() = default;

    static auto create_object() -> NullExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// AssignExpr
class AssignExpr : public Expr {
public:
    Token* left;
    Token* oper;
    Expr* right;

private:
    AssignExpr(Token* left, Token* oper, Expr* right);
    AssignExpr(AssignExpr& source) = default;
    auto operator = (AssignExpr& other) -> AssignExpr& = default;

public:
    ~AssignExpr();

    static auto create_object(Token* left, Token* oper, Expr* right) -> AssignExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// TernaryExpr
class TernaryExpr : public Expr {
public:
    Expr* condition;
    Expr* first;
    Expr* second;

private:
    TernaryExpr(Expr* condition, Expr* first, Expr* second);
    TernaryExpr(TernaryExpr& source) = default;
    auto operator = (TernaryExpr& other) -> TernaryExpr& = default;

public:
    ~TernaryExpr();

    static auto create_object(Expr* condition, Expr* first, Expr* second) -> TernaryExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};

// BinaryExpr
class BinaryExpr : public Expr {
public:
    Expr* left;
    Token* oper;
    Expr* right;

private:
    BinaryExpr(Expr* left, Token* oper, Expr* right);
    BinaryExpr(BinaryExpr& source) = default;
    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;

public:
    ~BinaryExpr();

    static auto create_object(Expr* left, Token* oper, Expr* right) -> BinaryExpr*;

    auto operator = (BinaryExpr& other) -> BinaryExpr& = default;
    virtual auto type() -> ExprType override;
};

// UnaryExpr
class UnaryExpr: public Expr {
public:
    Token* oper;
    Expr* operand;

private:
    UnaryExpr(Token* oper, Expr* operand);
    UnaryExpr(UnaryExpr& source) = default;
    auto operator = (UnaryExpr& other) -> UnaryExpr& = default;

public:
    ~UnaryExpr();

    static auto create_object(Token* oper, Expr* operand) -> UnaryExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};

// ExponentExpr
class ExponentExpr : public Expr {
public:
    Expr* left;
    Token* oper;
    Expr* right;

private:
    ExponentExpr(Expr* left, Token* oper, Expr* right);
    ExponentExpr(ExponentExpr& source) = default;
    auto operator = (ExponentExpr& other) -> ExponentExpr& = default;

public:
    ~ExponentExpr();

    static auto create_object(Expr* left, Token* oper, Expr* right) -> ExponentExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};

// GroupingExpr
class GroupingExpr : public Expr {
public:
    Expr* expression;

private:
    GroupingExpr(Expr* expression);
    GroupingExpr(GroupingExpr& source) = default;
    auto operator = (GroupingExpr& other) -> GroupingExpr& = default;

public:
    ~GroupingExpr();

    static auto create_object(Expr* expression) -> GroupingExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};

// PrimaryExpr
class PrimaryExpr : public Expr {
public:
    Token* token;

private:
    PrimaryExpr(Token* token);
    PrimaryExpr(PrimaryExpr& source) = default;
    auto operator = (PrimaryExpr& other) -> PrimaryExpr& = default;

public:
    ~PrimaryExpr();

    static auto create_object(Token* token) -> PrimaryExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;

};

// Forward declaration
class FunctionExpr;

// Forward declaration
class CallExpr;

// Forward declaration
class StructExpr;

// Forward declaration
class GetExpr;

// Forward declaration
class SetExpr;

// ExprVisitor
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual auto execute(std::vector<Expr*>& expressions) -> LeafObject* = 0;
    virtual auto visit_nullexpr(NullExpr* expr) -> LeafObject* = 0;
    virtual auto visit_assignexpr(AssignExpr* expr) -> LeafObject* = 0;
    virtual auto visit_ternaryexpr(TernaryExpr* expr) -> LeafObject* = 0;
    virtual auto visit_binaryexpr(BinaryExpr* expr) -> LeafObject* = 0;
    virtual auto visit_unaryexpr(UnaryExpr* expr) -> LeafObject* = 0;
    virtual auto visit_exponentexpr(ExponentExpr* expr) -> LeafObject* = 0;
    virtual auto visit_groupingexpr(GroupingExpr* expr) -> LeafObject* = 0;
    virtual auto visit_primaryexpr(PrimaryExpr* expr) -> LeafObject* = 0;
    virtual auto visit_functionexpr(FunctionExpr* expr) -> LeafObject* = 0;
    virtual auto visit_callexpr(CallExpr* expr) -> LeafObject* = 0;
    virtual auto visit_structexpr(StructExpr* expr) -> LeafObject* = 0;
    virtual auto visit_getexpr(GetExpr* expr) -> LeafObject* = 0;
    virtual auto visit_setexpr(SetExpr* expr) -> LeafObject* = 0;
};

#endif
