#include "parser.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

using namespace std::string_literals;

using enum TokenType;

Parser::Parser(const std::vector<const Token*>& tokens) :
    m_tokens { tokens }
    {
    }

Parser::~Parser() {
    for (const Stmt* statement : m_statements) {
        Stmt::delete_object(statement);
    }
}


// Public methods
auto Parser::statements() const -> const std::vector<const Stmt*>& {
    return m_statements;
}

auto Parser::parse() -> Parser& {
    while(is_at_end() == false && peek_token()->type() != k_eof) {
        m_statements.push_back(statement());
    }
    return *this;
}


// Private helper methods
auto Parser::is_at_end() -> bool {
    return m_current >= m_tokens.size();
}

auto Parser::move_current_left() -> void {
    if (m_current > 0) {
        m_current--;
    }
}

auto Parser::move_current_right() -> void {
    if (is_at_end() == false) {
        m_current++;
    }
}

auto Parser::get_token() -> const Token* {
    if (is_at_end() == false) {
        return m_tokens.at(m_current++);
    }
    return m_tokens.back();
}

auto Parser::peek_token() -> const Token* {
    if (is_at_end() == false) {
        return m_tokens.at(m_current);
    }
    return m_tokens.back();
}

auto Parser::peek_prev_token() -> const Token* {
    return m_tokens.at(m_current - 1);
}

auto Parser::match_token(std::initializer_list<TokenType> types) -> bool {
    TokenType type { peek_token()->type() };
    for (TokenType token_type : types ) {
        if (type == token_type) {
            move_current_right();
            return true;
        }
    }
    return false;
}

auto Parser::match_prev_token(std::initializer_list<TokenType> types) -> bool {
    TokenType type { peek_prev_token()->type() };
    for (TokenType token_type : types) {
        if (type == token_type) {
            return true;
        }
    }
    return false;
}

auto Parser::expect_token(TokenType type, const std::string& message) -> const Token* {
    if (match_token({ type })) {
        return peek_prev_token();
    }
    LeafError::instance()->add_parse_error(peek_prev_token()->line(), message);
    return nullptr;
}


// Private methods
auto Parser::statement() -> const Stmt* {
    const Token* token { get_token() };
    switch (token->type()) {
        case k_var: return varstmt();
        case k_const: return conststmt();
        case k_print: return printstmt();
        default: move_current_left(); return expressionstmt();
    }
}

auto Parser::varstmt() -> const Stmt* {
    const Token* identifier_token { expect_token(k_identifier, "An identifier is required after 'var' keyword."s) };

    const Expr* expr { NullExpr::create_object() };
    if (match_token({ k_equal })) {
        expr = expression();
    }

    expect_token(k_semicolon, "A statement must end with ';'."s);
    return VarStmt::create_object(identifier_token, expr);
}

auto Parser::conststmt() -> const Stmt* {
    const Token* identifier_token { expect_token(k_identifier, "An identifier is required after 'const' keyword."s) };

    const Expr* expr { NullExpr::create_object() };
    if (expect_token(k_equal, "Const declration must be provided an initializer."s)) {
        expr = expression();
        expect_token(k_semicolon, "A statement must end with ';'.");
        return ConstStmt::create_object(identifier_token, expr);
    }
    expect_token(k_semicolon, "A statement must end with ';'.");
    return nullptr;
}

auto Parser::printstmt() -> const Stmt* {
    const Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return PrintStmt::create_object(expr);
}

auto Parser::expressionstmt() -> const Stmt* {
    const Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'.");
    return ExpressionStmt::create_object(expr);
}

auto Parser::expression() -> const Expr* {
    return assign();
}

auto Parser::assign() -> const Expr* {
    if (match_token({ k_identifier })) {
        const Token* identifier { peek_prev_token() };
        if (match_token({ k_equal })) {
            const Token* oper { peek_prev_token() };
            const Expr* expr { assign() };
            return AssignExpr::create_object(identifier, oper, expr);
        }
        move_current_left();
    }
    return ternary();
}

auto Parser::ternary() -> const Expr* {
    const Expr* expr { or_expr() };

    while (match_token({ k_question })) {
        const Expr* first { ternary() };
        expect_token(k_colon, "':' is expected but not provided."s);
        const Expr* second { ternary() };
        expr = TernaryExpr::create_object(expr, first, second);
    }

    return expr;
}

auto Parser::or_expr() -> const Expr* {
    const Expr* expr { and_expr() };

    while (match_token({ k_or })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { and_expr() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::and_expr() -> const Expr* {
    const Expr* expr { equality() };

    while (match_token({ k_and })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { equality() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::equality() -> const Expr* {
    const Expr* expr { comparision() };

    while (match_token({ k_equal_equal, k_bang_equal })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { comparision() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::comparision() -> const Expr* {
    const Expr* expr { term() };

    while (match_token({ k_lesser, k_lesser_equal, k_greater, k_greater_equal })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { term() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::term() -> const Expr* {
    const Expr* expr { factor() };

    while (match_token({ k_plus, k_minus })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { factor() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::factor() -> const Expr* {
    const Expr* expr { unary() };

    while (match_token({ k_star, k_slash, k_percent })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { unary() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::unary() -> const Expr* {
    while (match_token({ k_bang, k_minus })) {
        const Token* oper { peek_prev_token() };
        const Expr* operand { exponent() };
        return UnaryExpr::create_object(oper, operand);
    }
    return exponent();
}

auto Parser::exponent() -> const Expr* {
    const Expr* expr { primary() };

    if (match_token({ k_star_star })) {
        const Token* oper { peek_prev_token() };
        const Expr* right { unary() };
        expr = ExponentExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::grouping() -> const Expr* {
    const Token* token { peek_token() };
    if (token->type() == k_right_paren) {
        LeafError::instance()->add_parse_error(token->line(), "An expression is required before ')'."s);
        move_current_right();
        return NullExpr::create_object();
    }

    token = peek_prev_token();
    const Expr* expr { expression() };
    expect_token(k_right_paren, "Each '(' must be accompanied by a matching ')'."s);
    return GroupingExpr::create_object(expr);
}

auto Parser::primary() -> const Expr* {
    const Token* token { get_token() };

    if (token->type() == k_eof) {
        return NullExpr::create_object();
    }

    if (token->type() == k_left_paren) {
        return grouping();
    }

    if (match_prev_token({ k_number, k_string, k_true, k_false, k_null, k_identifier })) {
        return PrimaryExpr::create_object(token);
    }

    return NullExpr::create_object();
}
