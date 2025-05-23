#ifndef leaf_expr_hpp
#define leaf_expr_hpp

#include "object.hpp"
#include "token.hpp"

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

struct Expr {
    static auto dest(Expr* expr) -> void;

    virtual ~Expr();
    virtual auto type() -> ExprType;
    virtual auto evaluate() -> Object*;
    virtual auto print() -> void;
};

struct TernaryExpr : public Expr {
    Expr* condition;
    Expr* first;
    Expr* second;

    ~TernaryExpr();

    static auto make(Expr* condition, Expr* first, Expr* second) -> TernaryExpr*;

    virtual auto type() -> ExprType override;
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

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
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

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
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

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
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

private:
    ExponentExpr(Token* oper, Expr* left, Expr* right);
    ExponentExpr(ExponentExpr&) = default;
    auto operator = (ExponentExpr&) -> ExponentExpr& = default;
};

struct LiteralExpr: public Expr {
    Token* value;

    static auto make(Token* value) -> LiteralExpr*;

    virtual auto type() -> ExprType override;
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

private:
    LiteralExpr(Token* vaue);
    LiteralExpr(LiteralExpr&) = default;
    auto operator = (LiteralExpr&) -> LiteralExpr& = default;
};

struct IdentifierExpr : public Expr {
    Token* name;

    static auto make(Token* name) -> IdentifierExpr*;

    virtual auto type() -> ExprType override;
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

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
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

private:
    GroupingExpr(Token* left_paren, Expr* expr);
    GroupingExpr(GroupingExpr&) = default;
    auto operator = (GroupingExpr&) -> GroupingExpr& = default;
};

struct NilExpr : public Expr {
    static auto make() -> NilExpr*;

    virtual auto type() -> ExprType override;
    virtual auto evaluate() -> Object* override;
    virtual auto print() -> void override;

private:
    NilExpr() = default;
    NilExpr(NilExpr&) = default;
    auto operator = (NilExpr&) -> NilExpr& = default;
};

#endif
