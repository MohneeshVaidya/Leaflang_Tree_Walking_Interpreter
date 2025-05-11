#include "expr.hpp"
#include "leaf_object.hpp"
#include "token.hpp"


// Expr
auto Expr::delete_object(Expr* object) -> void {
    delete object;
}


// NullExpr
auto NullExpr::create_object() -> NullExpr* {
    return new NullExpr { };
}

auto NullExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_nullexpr(this);
}

auto NullExpr::type() -> ExprType {
    return ExprType::k_null;
}


// AssignExpr
auto AssignExpr::create_object(Token* left, Token* oper, Expr* right) -> AssignExpr* {
    return new AssignExpr { left, oper, right };
}

AssignExpr::AssignExpr(Token* left, Token* oper, Expr* right) :
    left { Token::create_object(*left) },
    oper { Token::create_object(*oper) },
    right { right }
    {
    }

AssignExpr::~AssignExpr() {
    Token::delete_object(left);
    Token::delete_object(oper);
    Expr::delete_object(right);
}

auto AssignExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_assignexpr(this);
}

auto AssignExpr::type() -> ExprType {
    return ExprType::k_assign;
}


// TernaryExpr
TernaryExpr::TernaryExpr(Expr* condition, Expr* first, Expr* second) :
    condition { condition },
    first { first },
    second { second }
    {
    }

TernaryExpr::~TernaryExpr() {
    Expr::delete_object(condition);
    Expr::delete_object(first);
    Expr::delete_object(second);
}

auto TernaryExpr::create_object(Expr* condition, Expr* first, Expr* second) -> TernaryExpr* {
    return new TernaryExpr { condition, first, second };
}

auto TernaryExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_ternaryexpr(this);
}

auto TernaryExpr::type() -> ExprType {
    return ExprType::k_ternary;
}


// BinaryExpr
BinaryExpr::BinaryExpr(Expr* left, Token* oper, Expr* right) :
    left { left },
    oper { Token::create_object(*oper) },
    right { right }
    {
    }

BinaryExpr::~BinaryExpr() {
    Expr::delete_object(left);
    Token::delete_object(oper);
    Expr::delete_object(right);
}

auto BinaryExpr::create_object(Expr* left, Token* oper, Expr* right) -> BinaryExpr* {
    return new BinaryExpr { left, oper, right };
}

auto BinaryExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_binaryexpr(this);
}

auto BinaryExpr::type() -> ExprType {
    return ExprType::k_binary;
}


// UnaryExpr
UnaryExpr::UnaryExpr(Token* oper, Expr* operand) :
    oper { Token::create_object(*oper) },
    operand { operand }
    {
    }

UnaryExpr::~UnaryExpr() {
    Token::delete_object(oper);
    Expr::delete_object(operand);
}

auto UnaryExpr::create_object(Token* oper, Expr* operand) -> UnaryExpr* {
    return new UnaryExpr { oper, operand };
}

auto UnaryExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_unaryexpr(this);
}

auto UnaryExpr::type() -> ExprType {
    return ExprType::k_unary;
}


// ExponentExpr
ExponentExpr::ExponentExpr(Expr* left, Token* oper, Expr* right) :
    left { left },
    oper { Token::create_object(*oper) },
    right { right }
    {
    }

ExponentExpr::~ExponentExpr() {
    Expr::delete_object(left);
    Token::delete_object(oper);
    Expr::delete_object(right);
}

auto ExponentExpr::create_object(Expr* left, Token* oper, Expr* right) -> ExponentExpr* {
    return new ExponentExpr { left, oper, right };
}

auto ExponentExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_exponentexpr(this);
}

auto ExponentExpr::type() -> ExprType {
    return ExprType::k_exponent;
}


// GroupingExpr
GroupingExpr::GroupingExpr(Expr* expression) :
    expression { expression }
    {
    }

GroupingExpr::~GroupingExpr() {
    Expr::delete_object(expression);
}

auto GroupingExpr::create_object(Expr* expression) -> GroupingExpr* {
    return new GroupingExpr { expression };
}

auto GroupingExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_groupingexpr(this);
}

auto GroupingExpr::type() -> ExprType {
    return ExprType::k_grouping;
}


// PrimaryExpr
PrimaryExpr::PrimaryExpr(Token* token) :
    token { Token::create_object(*token) }
    {
    }

PrimaryExpr::~PrimaryExpr() {
    Token::delete_object(token);
}

auto PrimaryExpr::create_object(Token* token) -> PrimaryExpr* {
    return new PrimaryExpr { token };
}

auto PrimaryExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_primaryexpr(this);
}

auto PrimaryExpr::type() -> ExprType {
    return ExprType::k_primary;
}
