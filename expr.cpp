#include "expr.hpp"

using enum ExprType;

// class Expr
auto Expr::dest(Expr* expr) -> void {
    delete expr;
}


// class TernaryExpr
TernaryExpr::TernaryExpr(Expr* condition, Expr* first, Expr* second) :
    condition{ condition },
    first{ first },
    second{ second }
    {
    }

TernaryExpr::~TernaryExpr() {
    Expr::dest(condition);
    Expr::dest(first);
    Expr::dest(second);
}

auto TernaryExpr::make(Expr* condition, Expr* first, Expr* second) -> TernaryExpr* {
    return new TernaryExpr{ condition, first, second };
}

auto TernaryExpr::evaluate() -> Object* {
    throw 1;
}

auto TernaryExpr::type() -> ExprType {
    return EXPR_TERNARY;
}


// class BinaryExpr
BinaryExpr::BinaryExpr(Token* oper, Expr* left, Expr* right) :
    oper{ oper },
    left{ left },
    right{ right }
    {
    }

BinaryExpr::~BinaryExpr() {
    Expr::dest(left);
    Expr::dest(right);
}

auto BinaryExpr::make(Token* oper, Expr* left, Expr* right) -> BinaryExpr* {
    return new BinaryExpr{ oper, left, right };
}

auto BinaryExpr::evaluate() -> Object* {
    throw 1;
}

auto BinaryExpr::type() -> ExprType {
    return EXPR_BINARY;
}


// class UnaryExpr
UnaryExpr::UnaryExpr(Token* oper, Expr* expr) :
    oper{ oper },
    expr{ expr }
    {
    }

UnaryExpr::~UnaryExpr() {
    Expr::dest(expr);
}

auto UnaryExpr::make(Token* oper, Expr* expr) -> UnaryExpr* {
    return new UnaryExpr{ oper, expr };
}

auto UnaryExpr::evaluate() -> Object* {
    throw 1;
}

auto UnaryExpr::type() -> ExprType {
    return EXPR_UNARY;
}


// class ExponentExpr
ExponentExpr::ExponentExpr(Token* oper, Expr* left, Expr* right) :
    oper{ oper },
    left{ left },
    right{ right }
    {
    }

ExponentExpr::~ExponentExpr() {
    Expr::dest(left);
    Expr::dest(right);
}

auto ExponentExpr::make(Token* oper, Expr* left, Expr* right) -> ExponentExpr* {
    return new ExponentExpr{ oper, left, right };
}

auto ExponentExpr::evaluate() -> Object* {
    throw 1;
}

auto ExponentExpr::type() -> ExprType {
    return EXPR_EXPONENT;
}


// class LiteralExpr
LiteralExpr::LiteralExpr(Token* value) :
    value { value }
    {
    }

auto LiteralExpr::make(Token* value) -> LiteralExpr* {
    return new LiteralExpr { value };
}

auto LiteralExpr::evaluate() -> Object* {
    throw 1;
}

auto LiteralExpr::type() -> ExprType {
    return EXPR_LITERAL;
}


// class IdentifierExpr
IdentifierExpr::IdentifierExpr(Token* name) :
    name{ name }
    {
    }

auto IdentifierExpr::make(Token* name) -> IdentifierExpr* {
    return new IdentifierExpr{ name };
}

auto IdentifierExpr::evaluate() -> Object* {
    throw 1;
}

auto IdentifierExpr::type() -> ExprType {
    return EXPR_IDENTIFIER;
}


// class GroupingExpr
GroupingExpr::GroupingExpr(Token* left_paren, Expr* expr) :
    left_paren{ left_paren },
    expr{ expr }
    {
    }

GroupingExpr::~GroupingExpr() {
    Expr::dest(expr);
}

auto GroupingExpr::make(Token* left_paren, Expr* expr) -> GroupingExpr* {
    return new GroupingExpr{ left_paren, expr };
}

auto GroupingExpr::evaluate() -> Object* {
    throw 1;
}

auto GroupingExpr::type() -> ExprType {
    return EXPR_GROUPING;
}


// class NilExpr
auto NilExpr::make() -> NilExpr* {
    return new NilExpr{ };
}

auto NilExpr::evaluate() -> Object* {
    throw 1;
}

auto NilExpr::type() -> ExprType {
    return EXPR_NIL;
}
