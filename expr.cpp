#include "expr.hpp"

using enum ExprType;

// struct Expr
auto Expr::dest(Expr* expr) -> void {
    delete expr;
}


// struct TernaryExpr
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

auto TernaryExpr::type() -> ExprType {
    return EXPR_TERNARY;
}

auto TernaryExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_ternaryexpr(this);
}


// struct BinaryExpr
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

auto BinaryExpr::type() -> ExprType {
    return EXPR_BINARY;
}

auto BinaryExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_binaryexpr(this);
}


// struct UnaryExpr
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

auto UnaryExpr::type() -> ExprType {
    return EXPR_UNARY;
}

auto UnaryExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_unaryexpr(this);
}


// struct ExponentExpr
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

auto ExponentExpr::type() -> ExprType {
    return EXPR_EXPONENT;
}

auto ExponentExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_exponentexpr(this);
}


// struct LiteralExpr
LiteralExpr::LiteralExpr(Token* value) :
    value { value }
    {
    }

auto LiteralExpr::make(Token* value) -> LiteralExpr* {
    return new LiteralExpr { value };
}

auto LiteralExpr::type() -> ExprType {
    return EXPR_LITERAL;
}

auto LiteralExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_literalexpr(this);
}


// struct IdentifierExpr
IdentifierExpr::IdentifierExpr(Token* name) :
    name{ name }
    {
    }

auto IdentifierExpr::make(Token* name) -> IdentifierExpr* {
    return new IdentifierExpr{ name };
}

auto IdentifierExpr::type() -> ExprType {
    return EXPR_IDENTIFIER;
}

auto IdentifierExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_identifierexpr(this);
}


// struct GroupingExpr
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

auto GroupingExpr::type() -> ExprType {
    return EXPR_GROUPING;
}

auto GroupingExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_groupingexpr(this);
}


// struct NilExpr
auto NilExpr::make() -> NilExpr* {
    return new NilExpr{ };
}

auto NilExpr::type() -> ExprType {
    return EXPR_NIL;
}

auto NilExpr::accept(ExprVisitor* visitor) -> Object* {
    return visitor->visit_nilexpr(this);
}
