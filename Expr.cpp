#include "Expr.hpp"
#include "Token.hpp"

TernaryExpr::TernaryExpr(ExprPtr condition, ExprPtr first, ExprPtr second) :
    condition { condition },
    first { first },
    second { second }
    {
    }

BinaryExpr::BinaryExpr(TokenPtr oper, ExprPtr left, ExprPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

UnaryExpr::UnaryExpr(TokenPtr oper, ExprPtr expr) :
    oper { oper },
    expr { expr }
    {
    }

ExponentExpr::ExponentExpr(TokenPtr oper, ExprPtr left, ExprPtr right) :
    oper { oper },
    left { left },
    right { right }
    {
    }

GroupExpr::GroupExpr(ExprPtr expr) :
    expr { expr }
    {
    }

PrimaryExpr::PrimaryExpr(TokenPtr token) :
    token { token }
    {
    }

