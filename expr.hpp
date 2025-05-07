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
    k_variable,
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
public:
    NullExpr() = default;
    NullExpr(const NullExpr& source) = default;

    ~NullExpr() = default;

    static auto create_object() -> NullExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const NullExpr& other) -> NullExpr& = default;
};


// AssignExpr
class AssignExpr : public Expr {
public:
    const Token* left;
    const Token* oper;
    const Expr* right;

    static auto create_object(const Token* left, const Token* oper, const Expr* right) -> AssignExpr*;

    AssignExpr(const Token* left, const Token* oper, const Expr* right);
    AssignExpr(const AssignExpr& source) = default;

    ~AssignExpr();

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const AssignExpr& other) -> AssignExpr& = default;
};


// TernaryExpr
class TernaryExpr : public Expr {
public:
    const Expr* condition;
    const Expr* first;
    const Expr* second;


    TernaryExpr(const Expr* condition, const Expr* first, const Expr* second);
    TernaryExpr(const TernaryExpr& source) = default;

    ~TernaryExpr();

    static auto create_object(const Expr* condition, const Expr* first, const Expr* second) -> TernaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const TernaryExpr& other) -> TernaryExpr& = default;
};

// BinaryExpr
class BinaryExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;


    BinaryExpr(const Expr* left, const Token* oper, const Expr* right);
    BinaryExpr(const BinaryExpr& source) = default;

    ~BinaryExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> BinaryExpr*;

    auto operator = (const BinaryExpr& other) -> BinaryExpr& = default;
    virtual auto type() const -> ExprType override;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
};

// UnaryExpr
class UnaryExpr: public Expr {
public:
    const Token* oper;
    const Expr* operand;


    UnaryExpr(const Token* oper, const Expr* operand);
    UnaryExpr(const UnaryExpr& source) = default;

    ~UnaryExpr();

    static auto create_object(const Token* oper, const Expr* operand) -> UnaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const UnaryExpr& other) -> UnaryExpr& = default;
};

// ExponentExpr
class ExponentExpr : public Expr {
public:
    const Expr* left;
    const Token* oper;
    const Expr* right;


    ExponentExpr(const Expr* left, const Token* oper, const Expr* right);
    ExponentExpr(const ExponentExpr& source) = default;

    ~ExponentExpr();

    static auto create_object(const Expr* left, const Token* oper, const Expr* right) -> ExponentExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const ExponentExpr& other) -> ExponentExpr& = default;
};

// GroupingExpr
class GroupingExpr : public Expr {
public:
    const Expr* expression;


    GroupingExpr(const Expr* expression);
    GroupingExpr(const GroupingExpr& source) = default;

    ~GroupingExpr();

    static auto create_object(const Expr* expression) -> GroupingExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const GroupingExpr& other) -> GroupingExpr& = default;
};

// PrimaryExpr
class PrimaryExpr : public Expr {
public:
    const Token* token;

    PrimaryExpr(const Token* token);
    PrimaryExpr(const PrimaryExpr& source) = default;

    ~PrimaryExpr();

    static auto create_object(const Token* token) -> PrimaryExpr*;

    virtual auto accept(const ExprVisitor* visitor) const -> LeafObject* override;
    virtual auto type() const -> ExprType override;

    auto operator = (const PrimaryExpr& other) -> PrimaryExpr& = default;
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
