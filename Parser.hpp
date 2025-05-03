#ifndef PARSER_HPP
#define PARSER_HPP

#include "Expr.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

#include <cstdint>
#include <initializer_list>
#include <string_view>
#include <vector>

class Parser {
private:
    const std::vector<TokenPtr>& tokens;
    std::vector<ExprPtr> expressions { };
    uint32_t current { 0 };

    // Private methods
    bool is_at_end();
    TokenPtr get_token();
    TokenPtr peek_token();
    bool match_token(std::initializer_list<TokenType> types);
    void expect_token(TokenType type, std::string_view message);
    TokenPtr peek_previous_token();
    bool match_previous_token(std::initializer_list<TokenType> types);
    void move_current_left();
    void move_current_right();

    ExprPtr expression();
    ExprPtr term_expr();
    ExprPtr factor_expr();
    ExprPtr unary_expr();
    ExprPtr exponent_expr();
    ExprPtr primary_expr();

public:
    Parser(const std::vector<TokenPtr>& tokens);
    ~Parser() = default;

    const std::vector<ExprPtr>& parse();
};

#endif
