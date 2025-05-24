#ifndef leaf_expr_hpp
#define leaf_expr_hpp

#include "object.hpp"
#include "token.hpp"
#include <vector>

enum class ExprType {
    EXPR_TERNARY,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_EXPONENT,
    EXPR_IDENTIFIER,
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_NIL,
};

struct ExprVisitor;

struct Expr {
    static auto dest(Expr* expr) -> void;

    virtual ~Expr() = default;
    virtual auto type() -> ExprType = 0;
    virtual auto accept(ExprVisitor* visitor) -> Object* = 0;
};

struct TernaryExpr : public Expr {
    Expr* condition;
    Expr* first;
    Expr* second;

    ~TernaryExpr();

    static auto make(Expr* condition, Expr* first, Expr* second) -> TernaryExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    TernaryExpr(Expr* condition, Expr* first, Expr* second);
    TernaryExpr(TernaryExpr&) = default;
    auto operator = (TernaryExpr&) -> TernaryExpr& = default;
};

struct BinaryExpr : public Expr {
    Token* oper;
    Expr* left;
    Expr* right;

    ~BinaryExpr();

    static auto make(Token* oper, Expr* left, Expr* right) -> BinaryExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    BinaryExpr(Token* oper, Expr* left, Expr* right);
    BinaryExpr(BinaryExpr&) = default;
    auto operator = (BinaryExpr&) -> BinaryExpr& = default;
};

struct UnaryExpr : public Expr {
    Token* oper;
    Expr* expr;

    ~UnaryExpr();

    static auto make(Token* oper, Expr* expr) -> UnaryExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    UnaryExpr(Token* oper, Expr* expr);
    UnaryExpr(UnaryExpr&) = default;
    auto operator = (UnaryExpr&) -> UnaryExpr& = default;
};

struct ExponentExpr : public Expr {
    Token* oper;
    Expr* left;
    Expr* right;

    ~ExponentExpr();

    static auto make(Token* oper, Expr* left, Expr* right) -> ExponentExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    ExponentExpr(Token* oper, Expr* left, Expr* right);
    ExponentExpr(ExponentExpr&) = default;
    auto operator = (ExponentExpr&) -> ExponentExpr& = default;
};

struct LiteralExpr: public Expr {
    Token* value;

    static auto make(Token* value) -> LiteralExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    LiteralExpr(Token* vaue);
    LiteralExpr(LiteralExpr&) = default;
    auto operator = (LiteralExpr&) -> LiteralExpr& = default;
};

struct IdentifierExpr : public Expr {
    Token* name;

    static auto make(Token* name) -> IdentifierExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    IdentifierExpr(Token* name);
    IdentifierExpr(IdentifierExpr&) = default;
    auto operator = (IdentifierExpr&) -> IdentifierExpr& = default;
};

struct GroupingExpr : public Expr {
    Token* left_paren;
    Expr* expr;

    ~GroupingExpr();

    static auto make(Token* left_paren, Expr* expr) -> GroupingExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    GroupingExpr(Token* left_paren, Expr* expr);
    GroupingExpr(GroupingExpr&) = default;
    auto operator = (GroupingExpr&) -> GroupingExpr& = default;
};

struct NilExpr : public Expr {
    static auto make() -> NilExpr*;

    virtual auto type() -> ExprType override;
    virtual auto accept(ExprVisitor* visitor) -> Object* override;

private:
    NilExpr() = default;
    NilExpr(NilExpr&) = default;
    auto operator = (NilExpr&) -> NilExpr& = default;
};


// struct ExprVisitor
struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual auto execute(std::vector<Expr*>& stmts) -> void = 0;
    virtual auto visit_ternaryexpr(TernaryExpr* expr) -> Object* = 0;
    virtual auto visit_binaryexpr(BinaryExpr* expr) -> Object* = 0;
    virtual auto visit_unaryexpr(UnaryExpr* expr) -> Object* = 0;
    virtual auto visit_exponentexpr(ExponentExpr* expr) -> Object* = 0;
    virtual auto visit_literalexpr(LiteralExpr* expr) -> Object* = 0;
    virtual auto visit_identifierexpr(IdentifierExpr* expr) -> Object* = 0;
    virtual auto visit_groupingexpr(GroupingExpr* expr) -> Object* = 0;
    virtual auto visit_nilexpr(NilExpr* expr) -> Object* = 0;
};

#endif
