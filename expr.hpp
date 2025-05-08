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
};


// ExprVisitor forward declaration
class ExprVisitor;

// Expr classes
class Expr {
public:
    static auto delete_object(const Expr* object) -> void;

    virtual ~Expr() = default;
    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* = 0;
    virtual auto type() const -> ExprType = 0;
};

// NullExpr
class NullExpr: public Expr {
private:
    NullExpr() = default;
    NullExpr(const NullExpr& source) = default;
    auto operator = (const NullExpr& other) -> NullExpr& = default;

public:
    ~NullExpr() = default;

    static auto create_object() -> NullExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};


// AssignExpr
class AssignExpr : public Expr {
public:
    const Token* left;
    const Token* oper;
    const Expr* right;

private:
    AssignExpr(const Token* left, const Token* oper, const Expr* right);
    AssignExpr(const AssignExpr& source) = default;
    auto operator = (const AssignExpr& other) -> AssignExpr& = default;

public:
    ~AssignExpr();

    static auto create_object(const Token* left, const Token* oper, const Expr* right) -> AssignExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};


// TernaryExpr
class TernaryExpr : public Expr {
public:
    const Expr* condition;
    const Expr* first;
    const Expr* second;

private:
    TernaryExpr(const Expr* condition, const Expr* first, const Expr* second);
    TernaryExpr(const TernaryExpr& source) = default;
    auto operator = (const TernaryExpr& other) -> TernaryExpr& = default;

public:
    ~TernaryExpr();

    static auto create_object(const Expr* condition, const Expr* first, const Expr* second) -> TernaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};

// BinaryExpr
class BinaryExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;

private:
    BinaryExpr(const Expr* left, const Token* oper, const Expr* right);
    BinaryExpr(const BinaryExpr& source) = default;
    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;

public:
    ~BinaryExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> BinaryExpr*;

    auto operator = (const BinaryExpr& other) -> BinaryExpr& = default;
    virtual auto type() const -> ExprType override;
};

// UnaryExpr
class UnaryExpr: public Expr {
public:
    const Token* oper;
    const Expr* operand;

private:
    UnaryExpr(const Token* oper, const Expr* operand);
    UnaryExpr(const UnaryExpr& source) = default;
    auto operator = (const UnaryExpr& other) -> UnaryExpr& = default;

public:
    ~UnaryExpr();

    static auto create_object(const Token* oper, const Expr* operand) -> UnaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};

// ExponentExpr
class ExponentExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;

private:
    ExponentExpr(const Expr* left, const Token* oper, const Expr* right);
    ExponentExpr(const ExponentExpr& source) = default;
    auto operator = (const ExponentExpr& other) -> ExponentExpr& = default;

public:
    ~ExponentExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> ExponentExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};

// GroupingExpr
class GroupingExpr : public Expr {
public:
    const Expr* expression;

private:
    GroupingExpr(const Expr* expression);
    GroupingExpr(const GroupingExpr& source) = default;
    auto operator = (const GroupingExpr& other) -> GroupingExpr& = default;

public:
    ~GroupingExpr();

    static auto create_object(const Expr* expression) -> GroupingExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;
};

// PrimaryExpr
class PrimaryExpr : public Expr {
public:
    const Token* token;

private:
    PrimaryExpr(const Token* token);
    PrimaryExpr(const PrimaryExpr& source) = default;
    auto operator = (const PrimaryExpr& other) -> PrimaryExpr& = default;

public:
    ~PrimaryExpr();

    static auto create_object(const Token* token) -> PrimaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

};

// ExprVisitor
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual auto execute(const std::vector<const Expr*>& expressions) const -> LeafObject* = 0;
    virtual auto visit_nullexpr(const NullExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_assignexpr(const AssignExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_ternaryexpr(const TernaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_binaryexpr(const BinaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_unaryexpr(const UnaryExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_exponentexpr(const ExponentExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_groupingexpr(const GroupingExpr* expr) const -> LeafObject* = 0;
    virtual auto visit_primaryexpr(const PrimaryExpr* expr) const -> LeafObject* = 0;
};

#endif
