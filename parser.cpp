#include "parser.hpp"
#include "expr.hpp"
#include "leaf_error.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <utility>

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
    while(is_at_end() == false) {
        if (peek_token()->type() == k_eof) {
            break;
        }
        m_statements.push_back(top_statement());
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
    throw std::string { };
}

auto Parser::for_helper() -> const Stmt* {
    const Stmt* step_stmt { ExpressionStmt::create_object(expression()) };
    expect_token(k_left_brace, "'{' must be provided to start 'for' block."s);

    BlockStmt* block_stmt { const_cast<BlockStmt*>(dynamic_cast<const BlockStmt*>(blockstmt())) };
    block_stmt->statements.push_back(step_stmt);

    return block_stmt;
}

auto Parser::synchronize() -> void {
    const Token* curr_token { get_token() };
    while (true) {
        switch (curr_token->type()) {
            case k_print:
                [[fallthrough]];
            case k_println:
                [[fallthrough]];
            case k_var:
                [[fallthrough]];
            case k_const:
                [[fallthrough]];
            case k_left_brace:
                [[fallthrough]];
            case k_if:
                [[fallthrough]];
            case k_for:
                [[fallthrough]];
            case k_do:
                [[fallthrough]];
            case k_eof:
                move_current_left();
                return;
            default:
                curr_token = get_token();
        }
    }
}


// Private methods
auto Parser::top_statement() -> const Stmt* {
    try {
        return statement();
    } catch (const std::string&) {
        synchronize();
        return nullptr;
    }
}

auto Parser::statement() -> const Stmt* {
    const Token* token { get_token() };
    switch (token->type()) {
        case k_var: return varstmt();
        case k_const: return conststmt();
        case k_print: return printstmt();
        case k_println: return printlnstmt();
        case k_left_brace: return blockstmt();
        case k_if: return ifstmt();
        case k_for: return forstmt();
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

auto Parser::printlnstmt() -> const Stmt* {
    const Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return PrintlnStmt::create_object(expr);
}

auto Parser::expressionstmt() -> const Stmt* {
    const Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'.");
    return ExpressionStmt::create_object(expr);
}

auto Parser::blockstmt() -> const Stmt* {
    std::vector<const Stmt*> statements { };
    while (match_token({ k_right_brace }) == false) {
        statements.push_back(statement());
    }
    return BlockStmt::create_object(statements);
}

auto Parser::ifstmt() -> const Stmt* {
    std::vector<std::pair<const Expr*, const Stmt*>> statements { };
    const Expr* condition { expression() };
    if (expect_token(k_left_brace, "'{' is expected after 'if' condition."s)) {
        const Stmt* stmts { blockstmt() };
        statements.push_back(
            std::make_pair(condition, stmts));
    }
    while (match_token({ k_elseif })) {
        condition = expression();
        if (expect_token(k_left_brace, "'{' is expected after 'elseif' condition."s)) {
            const Stmt* stmts { blockstmt() };
            statements.push_back(
                std::make_pair(condition, stmts));
        }
    }
    if (match_token({ k_else })) {
        if (expect_token(k_left_brace, "'{' is expected after 'else'."s)) {
            const Stmt* stmts { blockstmt() };
            statements.push_back(
                std::make_pair(nullptr, stmts));
        }
    }
    return IfStmt::create_object(statements);
}

auto Parser::forstmt() -> const Stmt* {
    if (match_token({ k_left_brace })) {
        const Stmt* statements { blockstmt() };
        return ForStmt::create_object(nullptr, statements);
    } else if (match_token({ k_var })) {
        std::vector<const Stmt*> statements { };

        const Stmt* decl_stmt { varstmt() };
        const ExpressionStmt* condition { dynamic_cast<const ExpressionStmt*>(expressionstmt()) };

        const Stmt* block_stmt { for_helper() };

        statements.push_back(decl_stmt);
        statements.push_back(ForStmt::create_object(condition->expr, block_stmt));
        return BlockStmt::create_object(statements);
    } else {
        const uint32_t original_pos { m_current };
        uint32_t num_semi { 0 };
        while (match_token({ k_left_brace }) == false) {
            if (peek_token()->type() == k_semicolon) {
                num_semi = num_semi + 1;
            }
            move_current_right();
        }
        m_current = original_pos;
        if (num_semi == 0) {
            const Expr* condition { expression() };
            expect_token(k_left_brace, "'{' must be provided after for condition."s);
            const Stmt* statements { blockstmt() };
            return ForStmt::create_object(condition, statements);
        } else {
            std::vector<const Stmt*> statements { };

            const Stmt* assign_stmt { expressionstmt() };
            const ExpressionStmt* condition { dynamic_cast<const ExpressionStmt*>(expressionstmt()) };

            const Stmt* block_stmt { for_helper() };

            statements.push_back(assign_stmt);
            statements.push_back(ForStmt::create_object(condition->expr, block_stmt));
            return BlockStmt::create_object(statements);
        }
    }
    return nullptr;
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

    LeafError::instance()->add_parse_error(
        token->line(), std::format("An expression is expected but found '{}'.", token->lexeme()));
    move_current_left();
    return NullExpr::create_object();
}
