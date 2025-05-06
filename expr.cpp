#include "expr.hpp"
#include "leaf_object.hpp"
#include "token.hpp"


// Expr
auto Expr::delete_object(const Expr* object) -> void {
    delete object;
    object = nullptr;
}


// NullExpr
auto NullExpr::create_object() -> NullExpr* {
    return new NullExpr { };
}

auto NullExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_nullexpr(this);
}

auto NullExpr::type() const -> ExprType {
    return ExprType::k_null;
}


// TernaryExpr
TernaryExpr::TernaryExpr(const Expr* condition, const Expr* first, const Expr* second) :
    condition { condition },
    first { first },
    second { second }
    {
    }

TernaryExpr::~TernaryExpr() {
    delete condition;
    delete first;
    delete second;

    condition = nullptr;
    first = nullptr;
    second = nullptr;
}

auto TernaryExpr::create_object(const Expr* condition, const Expr* first, const Expr* second) -> TernaryExpr* {
    return new TernaryExpr { condition, first, second };
}

auto TernaryExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_ternaryexpr(this);
}

auto TernaryExpr::type() const -> ExprType {
    return ExprType::k_ternary;
}


// BinaryExpr
BinaryExpr::BinaryExpr(const Expr* left, const Token* oper, const Expr* right) :
    left { left },
    oper { Token::create_object(*oper) },
    right { right }
    {
    }

BinaryExpr::~BinaryExpr() {
    delete left;
    Token::delete_object(oper);
    delete right;
}

auto BinaryExpr::create_object(const Expr* left, const Token* oper, const Expr* right) -> BinaryExpr* {
    return new BinaryExpr { left, oper, right };
}

auto BinaryExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_binaryexpr(this);
}

auto BinaryExpr::type() const -> ExprType {
    return ExprType::k_binary;
}


// UnaryExpr
UnaryExpr::UnaryExpr(const Token* oper, const Expr* operand) :
    oper { Token::create_object(*oper) },
    operand { operand }
    {
    }

UnaryExpr::~UnaryExpr() {
    Token::delete_object(oper);
    delete operand;
}

auto UnaryExpr::create_object(const Token* oper, const Expr* operand) -> UnaryExpr* {
    return new UnaryExpr { oper, operand };
}

auto UnaryExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_unaryexpr(this);
}

auto UnaryExpr::type() const -> ExprType {
    return ExprType::k_unary;
}


// ExponentExpr
ExponentExpr::ExponentExpr(const Expr* left, const Token* oper, const Expr* right) :
    left { left },
    oper { Token::create_object(*oper) },
    right { right }
    {
    }

ExponentExpr::~ExponentExpr() {
    delete left;
    Token::delete_object(oper);
    delete right;
}

auto ExponentExpr::create_object(const Expr* left, const Token* oper, const Expr* right) -> ExponentExpr* {
    return new ExponentExpr { left, oper, right };
}

auto ExponentExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_exponentexpr(this);
}

auto ExponentExpr::type() const -> ExprType {
    return ExprType::k_exponent;
}


// GroupingExpr
GroupingExpr::GroupingExpr(const Expr* expression) :
    expression { expression }
    {
    }

GroupingExpr::~GroupingExpr() {
    delete expression;
    expression = nullptr;
}

auto GroupingExpr::create_object(const Expr* expression) -> GroupingExpr* {
    return new GroupingExpr { expression };
}

auto GroupingExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_groupingexpr(this);
}

auto GroupingExpr::type() const -> ExprType {
    return ExprType::k_grouping;
}


// PrimaryExpr
PrimaryExpr::PrimaryExpr(const Token* token) :
    token { Token::create_object(*token) }
    {
    }

PrimaryExpr::~PrimaryExpr() {
    Token::delete_object(token);
    token = nullptr;
}

auto PrimaryExpr::create_object(const Token* token) -> PrimaryExpr* {
    return new PrimaryExpr { token };
}

auto PrimaryExpr::accept(const ExprVisitor* visitor) const -> LeafObject* {
    return visitor->visit_primaryexpr(this);
}

auto PrimaryExpr::type() const -> ExprType {
    return ExprType::k_primary;
}
