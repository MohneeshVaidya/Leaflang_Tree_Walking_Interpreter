#include "Parser.hpp"
#include "Expr.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

#include <format>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

using enum TokenType;

Parser::Parser(const std::vector<TokenPtr>& tokens) :
    tokens { tokens }
    {
    }

const std::vector<ExprPtr>& Parser::parse() {
    while (is_at_end() == false) {
        expressions.push_back(expression());
    }
    return expressions;
}


// Private methods

bool Parser::is_at_end() {
    return current >= tokens.size() - 1;
}

TokenPtr Parser::get_token() {
    if (is_at_end() == false) {
        return tokens.at(current++);
    }
    return tokens.back();
}

TokenPtr Parser::peek_token() {
    if (is_at_end() == false) {
        return tokens.at(current);
    }
    return tokens.back();
}

bool Parser::match_token(std::initializer_list<TokenType> types) {
    TokenPtr token { peek_token() };
    for (TokenType type : types) {
        if (type == token->type) {
            return true;
        }
    }
    return false;
}

void Parser::expect_token(TokenType type, std::string_view message) {
    if (peek_token()->type == type) {
        move_current_right();
        return;
    }
    throw std::string { message };
}

TokenPtr Parser::peek_previous_token() {
    if (current > 0) {
        return tokens.at(current - 1);
    }
    return tokens.back();
}

bool Parser::match_previous_token(std::initializer_list<TokenType> types) {
    TokenType previous_type { peek_previous_token()->type };
    for (TokenType type : types) {
        if (type == previous_type) {
            return true;
        }
    }
    return false;
}

void Parser::move_current_left() {
    if (current > 0) {
        current--;
    }
}

void Parser::move_current_right() {
    if (is_at_end() == false) {
        current++;
    }
}

ExprPtr Parser::expression() {
    return term_expr();
}

ExprPtr Parser::term_expr() {
    ExprPtr expr { factor_expr() };

    while (match_token({ k_plus, k_minus })) {
        TokenPtr oper { get_token() };
        ExprPtr right { factor_expr() };
        expr = std::make_shared<BinaryExpr>(oper, expr, right);
    }

    return expr;
}

ExprPtr Parser::factor_expr() {
    ExprPtr expr { unary_expr() };

    while (match_token({ k_star, k_slash, k_percent })) {
        TokenPtr oper { get_token() };
        ExprPtr right { unary_expr() };
        expr = std::make_shared<BinaryExpr>(oper, expr, right);
    }

    return expr;
}

ExprPtr Parser::unary_expr() {
    if (match_token({ k_bang, k_minus })) {
        TokenPtr oper { get_token() };
        ExprPtr operand { exponent_expr() };
        ExprPtr expr { std::make_shared<UnaryExpr>(oper, operand) };
        return expr;
    }

    return exponent_expr();
}

ExprPtr Parser::exponent_expr() {
    ExprPtr expr { primary_expr() };

    while (match_token({ k_star_star })) {
        TokenPtr oper { get_token() };
        expr = std::make_shared<ExponentExpr>(oper, expr, expression());
    }

    return expr;
}

ExprPtr Parser::primary_expr() {
    TokenPtr token { get_token() };
    if (token == nullptr) {
        return nullptr;
    }

    ExprPtr expr { nullptr };

    if (token->type == k_left_paren) {
        expr = std::make_shared<GroupExpr>(expression());
        expect_token(k_right_paren, std::format("Error: [Near line {}] - A ')' is expected but not provided.", token->line));
        return expr;
    }

    if (match_previous_token({ k_number, k_string, k_null, k_true, k_false })) {
        expr = std::make_shared<PrimaryExpr>(token);
    }

    return expr;
}
