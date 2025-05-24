#include "parser.hpp"
#include "error.hpp"
#include "expr.hpp"
#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

using enum TokenType;
using enum error::ErrorType;

struct Parser {
    std::vector<Token*> tokens;
    std::vector<Expr*> stmts;
    uint32_t current;
};

Parser parser_instance{ };

static auto init(std::vector<Token*>& tokens) -> void {
    parser_instance.tokens = tokens;
    parser_instance.stmts = { };
    parser_instance.current = 0;
}

static auto tokens() -> std::vector<Token*>& {
    return parser_instance.tokens;
}

static auto stmts() -> std::vector<Expr*>& {
    return parser_instance.stmts;
}

static auto current() -> uint32_t& {
    return parser_instance.current;
}


// helper functions
static auto is_at_end() -> bool;
static auto get_token() -> Token*;
static auto peek_token() -> Token*;
static auto peek_prev_token() -> Token*;
static auto match_token(std::initializer_list<TokenType> types) -> bool;
static auto match_prev_token(std::initializer_list<TokenType> types) -> bool;
static auto expect_token(TokenType type, uint32_t line, std::string_view message) -> Token*;

static auto expression() -> Expr*;
static auto assignment() -> Expr*;
static auto ternary() -> Expr*;
static auto or_expr() -> Expr*;
static auto and_expr() -> Expr*;
static auto equality() -> Expr*;
static auto comparision() -> Expr*;
static auto term() -> Expr*;
static auto factor() -> Expr*;
static auto unary() -> Expr*;
static auto exponent() -> Expr*;
static auto primary() -> Expr*;
static auto grouping() -> Expr*;

namespace parser {
auto get_stmts(std::vector<Token*>& tokens) -> std::vector<Expr*>& {
    init(tokens);

    while (true) {
        if (is_at_end() || (peek_token()->type == TOKEN_EOF)) {
            break;
        }
        stmts().push_back(expression());
    }

    return stmts();
}

auto reset() -> void {
    for (Expr* stmt : stmts()) {
        Expr::dest(stmt);
    }
}
};


// helper functions implementations
static auto is_at_end() -> bool {
    return current() >= tokens().size();
}

static auto advance_current() -> void {
    if (is_at_end() == false) {
        current()++;
    }
}

static auto get_token() -> Token* {
    if (is_at_end()) {
        return tokens().back();
    }
    current()++;
    return tokens().at(current() - 1);
}

static auto peek_token() -> Token* {
    if (is_at_end()) {
        return tokens().back();
    }
    return tokens().at(current());
}

static auto peek_prev_token() -> Token* {
    return tokens().at(current() - 1);
}

static auto match_token(std::initializer_list<TokenType> types) -> bool {
    TokenType type { peek_token()->type };
    for (TokenType token_type : types) {
        if (type == token_type) {
            advance_current();
            return true;
        }
    }
    return false;
}

static auto match_prev_token(std::initializer_list<TokenType> types) -> bool {
    TokenType type { peek_prev_token()->type };
    for (TokenType token_type : types) {
        if (type == token_type) {
            return true;
        }
    }
    return false;
}

static auto expect_token(TokenType type, uint32_t line, std::string_view message) -> Token* {
    Token* token { get_token() };
    if (token->type != type) {
        error::add_parse_error(
            type,
            line,
            message
        );
        throw ERROR_PARSE;
    }
    return token;
}

static auto expression() -> Expr* {
    return assignment();
}

static auto assignment() -> Expr* {
    return ternary();
}

static auto ternary() -> Expr* {
    Expr* expr { or_expr() };
    if (match_token({ TOKEN_QUESTION })) {
        Token* oper { peek_prev_token() };
        Expr* first { ternary() };
        expect_token(TOKEN_COLON, oper->line, "in a ternary expression, ':' is required to separate two expressions.");
        Expr* second { ternary() };
        return TernaryExpr::make(expr, first, second);
    }
    return expr;
}

static auto or_expr() -> Expr* {
    Expr* left { and_expr() };
    while (match_token({ TOKEN_OR })) {
        Token* oper { peek_prev_token() };
        Expr* right { and_expr() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto and_expr() -> Expr* {
    Expr* left { equality() };
    while (match_token({ TOKEN_AND })) {
        Token* oper { peek_prev_token() };
        Expr* right { equality() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto equality() -> Expr* {
    Expr* left { comparision() };
    while (match_token({ TOKEN_EQUAL_EQUAL, TOKEN_BANG_EQUAL })) {
        Token* oper { peek_prev_token() };
        Expr* right { comparision() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto comparision() -> Expr* {
    Expr* left { term() };
    while (match_token({ TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL })) {
        Token* oper { peek_prev_token() };
        Expr* right { term() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto term() -> Expr* {
    Expr* left { factor() };
    while (match_token({ TOKEN_PLUS, TOKEN_MINUS })) {
        Token* oper { peek_prev_token() };
        Expr* right { factor() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto factor() -> Expr* {
    Expr* left { exponent() };
    while (match_token({ TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT })) {
        Token* oper { peek_prev_token() };
        Expr* right { exponent() };
        left = BinaryExpr::make(oper, left, right);
    }
    return left;
}

static auto exponent() -> Expr* {
    Expr* left { unary() };
    if (match_token({ TOKEN_STAR_STAR })) {
        Token* oper { peek_prev_token() };
        Expr* right { exponent() };
        return ExponentExpr::make(oper, left, right);
    }
    return left;
}

static auto unary() -> Expr* {
    if (match_token({ TOKEN_BANG, TOKEN_MINUS })) {
        Token* oper { peek_prev_token() };
        Expr* expr { unary() };
        return UnaryExpr::make(oper, expr);
    }
    return primary();
}

static auto primary() -> Expr* {
    Token* token { get_token() };
    if (token->type == TOKEN_EOF) {
        return nullptr;
    }

    if (token->type == TOKEN_NIL) {
        return NilExpr::make();
    }

    if (match_prev_token({ TOKEN_TRUE, TOKEN_FALSE, TOKEN_NUMBER, TOKEN_STRING })) {
        return LiteralExpr::make(token);
    }

    if (token->type == TOKEN_IDENTIFIER) {
        return IdentifierExpr::make(token);
    }

    if (token->type == TOKEN_LEFT_PAREN) {
        return grouping();
    }

    return NilExpr::make();
}

static auto grouping() -> Expr* {
    Token* left_paren { peek_prev_token() };
    Expr* expr { expression() };
    expect_token(TOKEN_RIGHT_PAREN, left_paren->line, "'(' should always be accompanied with ')'.");
    return GroupingExpr::make(left_paren, expr);
}
