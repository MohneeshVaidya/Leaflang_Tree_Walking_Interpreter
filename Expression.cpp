#include "Expression.hpp"
#include "Token.hpp"

TernaryExpr::TernaryExpr(ExpressionPtr condition, ExpressionPtr first, ExpressionPtr second) :
    condition { condition },
    first { first },
    second { second }
    {
    }

EqualityExpr::EqualityExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

ComparisionExpr::ComparisionExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

TermExpr::TermExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

FactorExpr::FactorExpr(TokenPtr oper, ExpressionPtr left, ExpressionPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

UnaryExpr::UnaryExpr(TokenPtr oper, ExpressionPtr expr) :
    oper { oper },
    expr { expr }
    {
    }

ExponentExpr::ExponentExpr(ExpressionPtr left, ExpressionPtr right) :
    left { left },
    right { right }
    {
    }

GroupExpr::GroupExpr(ExpressionPtr expr) :
    expr { expr }
    {
    }

PrimaryExpr::PrimaryExpr(TokenPtr token) :
    token { token }
    {
    }

