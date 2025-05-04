#include "parser.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "token_type.hpp"

using enum TokenType;

Parser::Parser(const std::vector<const Token*>& tokens) :
    m_tokens { tokens }
    {
    }

Parser::~Parser() {
    for (const Expr* expression : m_expressions) {
        Expr::delete_object(expression);
    }
}


// Public methods
auto Parser::expressions() const -> const std::vector<const Expr*>& {
    return m_expressions;
}

auto Parser::parse() -> Parser& {
    while(is_at_end() == false) {
        m_expressions.push_back(expression());
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

auto Parser::expect_token(TokenType type, const uint32_t line, const std::string& message) -> void {
    if (match_token({ type })) {
        return;
    }
    LeafError::instance()->add_parse_error(line, message);
}

// Private methods
auto Parser::expression() -> const Expr* {
    return term();
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

auto Parser::primary() -> const Expr* {
    const Token* token { get_token() };

    if (token->type() == k_eof) {
        return nullptr;
    }

    if (token->type() == k_left_paren) {
        const Expr* expr { expression() };
        expect_token(k_right_paren, token->line(), "Token '(' must have a trailing ')'");
        return expr;
    }

    if (match_prev_token({ k_number, k_string, k_true, k_false, k_null })) {
        return PrimaryExpr::create_object(token);
    }

    return nullptr;
}
