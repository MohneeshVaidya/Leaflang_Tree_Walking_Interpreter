#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Token.hpp"

#include <memory>
#include <string_view>

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

    TernaryExpr(const ExpressionPtr condition, const ExpressionPtr first, const ExpressionPtr second);
};

class EqualityExpr : public Expression {
public:
    Token oper;
    ExpressionPtr left;
    ExpressionPtr right;

    EqualityExpr(const Token& oper, const ExpressionPtr left, const ExpressionPtr right);
};

class ComparisionExpr : public Expression {
public:
    Token oper;
    ExpressionPtr left;
    ExpressionPtr right;

    ComparisionExpr(const Token& oper, const ExpressionPtr left, const ExpressionPtr right);
};

class TermExpr : public Expression {
public:
    Token oper;
    ExpressionPtr left;
    ExpressionPtr right;

    TermExpr(const Token& oper, const ExpressionPtr left, const ExpressionPtr right);
};

class FactorExpr : public Expression {
public:
    Token oper;
    ExpressionPtr left;
    ExpressionPtr right;

    FactorExpr(const Token& oper, const ExpressionPtr left, const ExpressionPtr right);
};

class UnaryExpr : public Expression {
public:
    Token oper;
    ExpressionPtr expr;

    UnaryExpr(const Token& oper, const ExpressionPtr expr);
};

class ExponentExpr : public Expression {
public:
    ExpressionPtr first;
    ExpressionPtr second;

    ExponentExpr(const ExpressionPtr first, const ExpressionPtr second);
};

class GroupExpr : public Expression {
public:
    ExpressionPtr expr;

    GroupExpr(const ExpressionPtr expr);
};

class PrimaryExpr : public Expression {
public:
    Token token;
    std::string lexeme;

    PrimaryExpr(const Token& oper, std::string_view lexeme);
};

#endif
