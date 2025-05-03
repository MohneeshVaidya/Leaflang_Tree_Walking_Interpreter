#ifndef EXPR_HPP
#define EXPR_HPP

#include "Token.hpp"

#include <memory>

class Expr;

using ExprPtr = std::shared_ptr<Expr>;

class Expr {
};

class TernaryExpr : public Expr {
public:
    ExprPtr condition;
    ExprPtr first;
    ExprPtr second;

    TernaryExpr(ExprPtr condition, ExprPtr first, ExprPtr second);
};

class BinaryExpr : public Expr {
public:
    TokenPtr oper;
    ExprPtr left;
    ExprPtr right;

    BinaryExpr(TokenPtr oper, ExprPtr left, ExprPtr right);
};

class UnaryExpr : public Expr {
public:
    TokenPtr oper;
    ExprPtr expr;

    UnaryExpr(TokenPtr oper, ExprPtr expr);
};

class ExponentExpr : public Expr {
public:
    TokenPtr oper;
    ExprPtr left;
    ExprPtr right;

    ExponentExpr(TokenPtr oper, ExprPtr left, ExprPtr right);
};

class GroupExpr : public Expr {
public:
    ExprPtr expr;

    GroupExpr(ExprPtr expr);
};

class PrimaryExpr : public Expr {
public:
    TokenPtr token;

    PrimaryExpr(TokenPtr token);
};

using TernaryExprPtr = std::shared_ptr<TernaryExpr>;
using BinaryExprPtr = std::shared_ptr<BinaryExpr>;
using UnaryExprPtr = std::shared_ptr<UnaryExpr>;
using ExponentExprPtr = std::shared_ptr<ExponentExpr>;
using GroupExprPtr = std::shared_ptr<GroupExpr>;
using PrimaryExprPtr = std::shared_ptr<PrimaryExpr>;

#endif