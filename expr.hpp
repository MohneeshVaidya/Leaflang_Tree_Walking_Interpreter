#ifndef EXPR_HPP
#define EXPR_HPP

#include "leaf_object.hpp"
#include "token.hpp"
#include <vector>


// ExprVisitor forward declaration
class ExprVisitor;


// Expr classes
class Expr {
public:
    static auto delete_object(const Expr* object) -> void;

    virtual ~Expr() = default;
    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* = 0;
};

class TernaryExpr : public Expr {
public:
    const Expr* condition;
    const Expr* first;
    const Expr* second;

public:
    TernaryExpr(const Expr* condition, const Expr* first, const Expr* second);
    TernaryExpr(const TernaryExpr& source) = default;

    ~TernaryExpr();

    static auto create_object(const Expr* condition, const Expr* first, const Expr* second) -> TernaryExpr*;

    auto operator = (const TernaryExpr& other) -> TernaryExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

class BinaryExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;

public:
    BinaryExpr(const Expr* left, const Token* oper, const Expr* right);
    BinaryExpr(const BinaryExpr& source) = default;

    ~BinaryExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> BinaryExpr*;

    auto operator = (const BinaryExpr& other) -> BinaryExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

class UnaryExpr: public Expr {
public:
    const Token* oper;
    const Expr* operand;

public:
    UnaryExpr(const Token* oper, const Expr* operand);
    UnaryExpr(const UnaryExpr& source) = default;

    ~UnaryExpr();

    static auto create_object(const Token* oper, const Expr* operand) -> UnaryExpr*;

    auto operator = (const UnaryExpr& other) -> UnaryExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

class ExponentExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;

public:
    ExponentExpr(const Expr* left, const Token* oper, const Expr* right);
    ExponentExpr(const ExponentExpr& source) = default;

    ~ExponentExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> ExponentExpr*;

    auto operator = (const ExponentExpr& other) -> ExponentExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

class GroupingExpr : public Expr {
public:
    const Expr* expression;

public:
    GroupingExpr(const Expr* expression);
    GroupingExpr(const GroupingExpr& source) = default;

    ~GroupingExpr();

    static auto create_object(const Expr* expression) -> GroupingExpr*;

    auto operator = (const GroupingExpr& other) -> GroupingExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

class PrimaryExpr : public Expr {
public:
    const Token* token;

public:
    PrimaryExpr(const Token* token);
    PrimaryExpr(const PrimaryExpr& source) = default;

    ~PrimaryExpr();

    static auto create_object(const Token* token) -> PrimaryExpr*;

    auto operator = (const PrimaryExpr& other) -> PrimaryExpr& = default;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};


// ExprVisitor
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual auto execute(const std::vector<const Expr*>& expressions) const -> LeafObject* = 0;
    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* = 0;
};

#endif
