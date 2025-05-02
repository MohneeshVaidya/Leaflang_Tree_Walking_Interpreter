#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Token.hpp"

#include <memory>

class Expression;

using ExpressionPtr = std::shared_ptr<Expression>;

class Expression {
public:
    // virtual ExpressionPtr accept() = 0;
};

class TernaryExpr : public Expression {
public:
    ExpressionPtr condition;
    ExpressionPtr first;
    ExpressionPtr second;

    TernaryExpr(ExpressionPtr condition, ExpressionPtr first, ExpressionPtr second);
};

class EqualityExpr : public Expression {
public:
    TokenPtr oper;
    ExpressionPtr left;
    ExpressionPtr right;

    EqualityExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right);
};

class ComparisionExpr : public Expression {
public:
    TokenPtr oper;
    ExpressionPtr left;
    ExpressionPtr right;

    ComparisionExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right);
};

class TermExpr : public Expression {
public:
    TokenPtr oper;
    ExpressionPtr left;
    ExpressionPtr right;

    TermExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right);
};

class FactorExpr : public Expression {
public:
    TokenPtr oper;
    ExpressionPtr left;
    ExpressionPtr right;

    FactorExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right);
};

class UnaryExpr : public Expression {
public:
    TokenPtr oper;
    ExpressionPtr expr;

    UnaryExpr(TokenPtr oper, ExpressionPtr expr);
};

class ExponentExpr : public Expression {
public:
    ExpressionPtr left;
    ExpressionPtr right;

    ExponentExpr(ExpressionPtr left, ExpressionPtr right);
};

class GroupExpr : public Expression {
public:
    ExpressionPtr expr;

    GroupExpr(ExpressionPtr expr);
};

class PrimaryExpr : public Expression {
public:
    TokenPtr token;

    PrimaryExpr(TokenPtr token);
};

#endif
