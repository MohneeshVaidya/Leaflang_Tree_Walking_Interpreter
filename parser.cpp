#include "parser.hpp"
#include "expr.hpp"
#include "leaf_function.hpp"
#include "leaf_error.hpp"
#include "leaf_struct.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <utility>

using namespace std::string_literals;

using enum TokenType;

Parser::Parser(std::vector<Token*>& tokens) :
    m_tokens { tokens }
    {
    }

Parser::~Parser() {
    for (Stmt* statement : m_statements) {
        Stmt::delete_object(statement);
    }
}


// Public methods
auto Parser::statements() -> std::vector<Stmt*>& {
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

auto Parser::get_token() -> Token* {
    if (is_at_end() == false) {
        return m_tokens.at(m_current++);
    }
    return m_tokens.back();
}

auto Parser::peek_token() -> Token* {
    if (is_at_end() == false) {
        return m_tokens.at(m_current);
    }
    return m_tokens.back();
}

auto Parser::peek_prev_token() -> Token* {
    return m_tokens.at(m_current - 1);
}

auto Parser::peek_next_token() -> Token* {
    move_current_right();
    Token* token { peek_token() };
    move_current_left();
    return token;
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

auto Parser::match_next_token(std::initializer_list<TokenType> types) -> bool {
    TokenType type { peek_next_token()->type() };
    for (auto token_type : types) {
        if (type == token_type) {
            return true;
        }
    }
    return false;
}

auto Parser::expect_token(TokenType type, const std::string& message) -> Token* {
    if (match_token({ type })) {
        return peek_prev_token();
    }
    LeafError::instance()->add_parse_error(peek_prev_token()->line(), message);
    throw std::string { };
}

auto Parser::for_helper() -> Stmt* {
    stack.top() = expression();
    Stmt* step_stmt { ExpressionStmt::create_object(stack.top()) };
    expect_token(k_left_brace, "'{' must be provided to start 'for' block."s);

    BlockStmt* block_stmt { const_cast<BlockStmt*>(dynamic_cast<BlockStmt*>(blockstmt())) };
    block_stmt->statements.push_back(step_stmt);

    return block_stmt;
}

auto Parser::struct_field_helper(std::vector<Token*>& fields) -> void {
    while (peek_token()->type() == k_identifier && peek_next_token()->type() == k_semicolon) {
        fields.push_back(get_token());
        get_token();
    }
}

auto Parser::struct_method_helper(std::vector<FunctionExpr*>& methods) -> void {
    if (match_token({ k_construct }) && peek_token()->type() == k_left_paren) {
        methods.push_back(dynamic_cast<FunctionExpr*>(function(peek_prev_token())));
        match_token({ k_semicolon });
        struct_method_helper(methods);
    }
    if (match_token({ k_identifier }) && peek_token()->type() == k_left_paren) {
        methods.push_back(dynamic_cast<FunctionExpr*>(function(peek_prev_token())));
        match_token({ k_semicolon });
        struct_method_helper(methods);
    }
}

auto Parser::synchronize() -> void {
    Token* curr_token { get_token() };
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
auto Parser::top_statement() -> Stmt* {
    try {
        return statement();
    } catch (const std::string&) {
        synchronize();
        return nullptr;
    }
}

auto Parser::statement() -> Stmt* {
    Token* token { get_token() };
    switch (token->type()) {
        case k_var: return varstmt();
        case k_const: return conststmt();
        case k_print: return printstmt();
        case k_println: return printlnstmt();
        case k_left_brace: return blockstmt();
        case k_if: return ifstmt();
        case k_for: return forstmt();
        case k_break: return breakstmt();
        case k_continue: return continuestmt();
        case k_return: return returnstmt();
        default: move_current_left(); return expressionstmt();
    }
}

auto Parser::varstmt() -> Stmt* {
    Token* identifier_token { expect_token(k_identifier, "An identifier is required after 'var' keyword."s) };

    Expr* expr { NullExpr::create_object() };
    if (match_token({ k_equal })) {
        expr = expression();
    }

    expect_token(k_semicolon, "A statement must end with ';'."s);
    return VarStmt::create_object(identifier_token, expr);
}

auto Parser::conststmt() -> Stmt* {
    Token* identifier_token { expect_token(k_identifier, "An identifier is required after 'const' keyword."s) };

    Expr* expr { NullExpr::create_object() };
    if (expect_token(k_equal, "Const declration must be provided an initializer."s)) {
        expr = expression();
        expect_token(k_semicolon, "A statement must end with ';'.");
        return ConstStmt::create_object(identifier_token, expr);
    }
    expect_token(k_semicolon, "A statement must end with ';'.");
    return nullptr;
}

auto Parser::printstmt() -> Stmt* {
    Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return PrintStmt::create_object(expr);
}

auto Parser::printlnstmt() -> Stmt* {
    Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return PrintlnStmt::create_object(expr);
}

auto Parser::expressionstmt() -> Stmt* {
    if (match_token({ k_struct })) {
        return ExpressionStmt::create_object(structexpr());
    }
    Expr* expr { expression() };
    expect_token(k_semicolon, "A statement must end with ';'.");
    return ExpressionStmt::create_object(expr);
}

auto Parser::blockstmt() -> Stmt* {
    std::vector<Stmt*> statements { };
    while (match_token({ k_right_brace }) == false) {
        statements.push_back(statement());
    }
    return BlockStmt::create_object(statements);
}

auto Parser::ifstmt() -> Stmt* {
    std::vector<std::pair<Expr*, Stmt*>> statements { };
    Expr* condition { expression() };
    if (expect_token(k_left_brace, "'{' is expected after 'if' condition."s)) {
        Stmt* stmts { blockstmt() };
        statements.push_back(
            std::make_pair(condition, stmts));
    }
    while (match_token({ k_elseif })) {
        condition = expression();
        if (expect_token(k_left_brace, "'{' is expected after 'elseif' condition."s)) {
            Stmt* stmts { blockstmt() };
            statements.push_back(
                std::make_pair(condition, stmts));
        }
    }
    if (match_token({ k_else })) {
        if (expect_token(k_left_brace, "'{' is expected after 'else'."s)) {
            Stmt* stmts { blockstmt() };
            statements.push_back(
                std::make_pair(nullptr, stmts));
        }
    }
    return IfStmt::create_object(statements);
}

auto Parser::forstmt() -> Stmt* {
    stack.push(nullptr);
    Stmt* to_return { };

    if (match_token({ k_left_brace })) {
        Stmt* statements { blockstmt() };
        to_return = ForStmt::create_object(nullptr, statements);
    } else if (match_token({ k_var })) {
        std::vector<Stmt*> statements { };

        Stmt* decl_stmt { varstmt() };
        ExpressionStmt* condition { dynamic_cast<ExpressionStmt*>(expressionstmt()) };

        Stmt* block_stmt { for_helper() };

        statements.push_back(decl_stmt);
        statements.push_back(ForStmt::create_object(condition->expr, block_stmt));
        to_return = BlockStmt::create_object(statements);
    } else {
        uint32_t original_pos { m_current };
        uint32_t num_semi { 0 };
        while (match_token({ k_left_brace }) == false) {
            if (peek_token()->type() == k_semicolon) {
                num_semi = num_semi + 1;
            }
            move_current_right();
        }
        m_current = original_pos;
        if (num_semi == 0) {
            Expr* condition { expression() };
            expect_token(k_left_brace, "'{' must be provided after for condition."s);
            Stmt* statements { blockstmt() };
            to_return = ForStmt::create_object(condition, statements);
        } else {
            std::vector<Stmt*> statements { };

            Stmt* assign_stmt { expressionstmt() };
            ExpressionStmt* condition { dynamic_cast<ExpressionStmt*>(expressionstmt()) };

            Stmt* block_stmt { for_helper() };

            statements.push_back(assign_stmt);
            statements.push_back(ForStmt::create_object(condition->expr, block_stmt));
            to_return = BlockStmt::create_object(statements);
        }
    }
    stack.pop();
    return to_return;
}

auto Parser::breakstmt() -> Stmt* {
    uint32_t line { peek_prev_token()->line() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return BreakStmt::create_object(line);
}

auto Parser::continuestmt() -> Stmt* {
    uint32_t line { peek_prev_token()->line() };
    expect_token(k_semicolon, "A statement must end with ';'."s);

    Expr* step_expr { nullptr };
    if (stack.empty() == false) {
        step_expr = stack.top();
    }

    Stmt* stmt { ContinueStmt::create_object(line, step_expr) };
    return stmt;
}

auto Parser::returnstmt() -> Stmt* {
    Token* token { peek_prev_token() };
    if (match_token({ k_semicolon })) {
        return ReturnStmt::create_object(token, NullExpr::create_object());
    }
    Expr* value { expression() };
    expect_token(k_semicolon, "A statement must end with ';'."s);
    return ReturnStmt::create_object(token, value);
}


auto Parser::expression() -> Expr* {
    return assign();
}

auto Parser::assign() -> Expr* {
    if (match_token({ k_identifier })) {
        Token* identifier { peek_prev_token() };
        if (match_token({ k_equal })) {
            Token* oper { peek_prev_token() };
            Expr* expr { assign() };
            return AssignExpr::create_object(identifier, oper, expr);
        } else if (match_token({ k_dot })) {
            return get(GetExpr::create_object(identifier, nullptr, nullptr, nullptr));
        }
        move_current_left();
    } else if (match_token({ k_function })) {
        return function(nullptr);
    }
    return ternary();
}

auto Parser::ternary() -> Expr* {
    Expr* expr { or_expr() };

    while (match_token({ k_question })) {
        Expr* first { ternary() };
        expect_token(k_colon, "':' is expected."s);
        Expr* second { ternary() };
        expr = TernaryExpr::create_object(expr, first, second);
    }

    return expr;
}

auto Parser::or_expr() -> Expr* {
    Expr* expr { and_expr() };

    while (match_token({ k_or })) {
        Token* oper { peek_prev_token() };
        Expr* right { and_expr() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::and_expr() -> Expr* {
    Expr* expr { equality() };

    while (match_token({ k_and })) {
        Token* oper { peek_prev_token() };
        Expr* right { equality() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::equality() -> Expr* {
    Expr* expr { comparision() };

    while (match_token({ k_equal_equal, k_bang_equal })) {
        Token* oper { peek_prev_token() };
        Expr* right { comparision() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::comparision() -> Expr* {
    Expr* expr { term() };

    while (match_token({ k_lesser, k_lesser_equal, k_greater, k_greater_equal })) {
        Token* oper { peek_prev_token() };
        Expr* right { term() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::term() -> Expr* {
    Expr* expr { factor() };

    while (match_token({ k_plus, k_minus })) {
        Token* oper { peek_prev_token() };
        Expr* right { factor() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::factor() -> Expr* {
    Expr* expr { unary() };

    while (match_token({ k_star, k_slash, k_percent })) {
        Token* oper { peek_prev_token() };
        Expr* right { unary() };
        expr = BinaryExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::unary() -> Expr* {
    while (match_token({ k_bang, k_minus })) {
        Token* oper { peek_prev_token() };
        Expr* operand { exponent() };
        return UnaryExpr::create_object(oper, operand);
    }
    return exponent();
}

auto Parser::exponent() -> Expr* {
    Expr* expr { primary() };

    if (match_token({ k_star_star })) {
        Token* oper { peek_prev_token() };
        Expr* right { unary() };
        expr = ExponentExpr::create_object(expr, oper, right);
    }

    return expr;
}

auto Parser::get(Expr* left_expr) -> Expr* {
    Token* right { expect_token(k_identifier, "An identifier is required after '.'."s) };

    GetExpr* expr { };
    if (match_token({ k_left_paren })) {
        expr = GetExpr::create_object(nullptr, left_expr, right, call(right));
    } else {
        expr = GetExpr::create_object(nullptr, left_expr, right, nullptr);
    }

    if (match_token({ k_dot })) {
        return get(expr);
    }

    if (match_token({ k_equal })) {
        Token* oper { peek_prev_token() };
        return SetExpr::create_object(expr, oper, expression());
    }

    return expr;
}

auto Parser::call(Token* identifier) -> Expr* {
    std::vector<Expr*> arguments { };
    while (match_token({ k_right_paren }) == false) {
        arguments.push_back(expression());
        match_token({ k_comma });
    }
    CallExpr* call_expr { CallExpr::create_object(identifier, arguments) };
    if (match_token({ k_left_paren })) {
        return call(call_expr);
    }
    return call_expr;
}

auto Parser::call(Expr* function_expr) -> Expr* {
    std::vector<Expr*> arguments { };
    while (match_token({ k_right_paren }) == false) {
        arguments.push_back(expression());
        match_token({ k_comma });
    }
    CallExpr* call_expr { CallExpr::create_object(function_expr, arguments) };
    if (match_token({ k_left_paren })) {
        return call(call_expr);
    }
    return call_expr;
}

auto Parser::grouping() -> Expr* {
    Token* token { peek_token() };
    if (token->type() == k_right_paren) {
        LeafError::instance()->add_parse_error(token->line(), "An expression is required before ')'."s);
        move_current_right();
        return NullExpr::create_object();
    }

    token = peek_prev_token();
    Expr* expr { expression() };
    expect_token(k_right_paren, "Each '(' must be accompanied by a matching ')'."s);
    return GroupingExpr::create_object(expr);
}

auto Parser::primary() -> Expr* {
    Token* token { get_token() };

    if (token->type() == k_eof) {
        return NullExpr::create_object();
    }

    if (token->type() == k_left_paren) {
        return grouping();
    }

    if (match_prev_token({ k_number, k_string, k_true, k_false, k_null })) {
        return PrimaryExpr::create_object(token);
    }

    if (match_prev_token({ k_identifier })) {
        if (match_token({ k_left_paren })) {
            return call(token);
        }
        return PrimaryExpr::create_object(token);
    }

    LeafError::instance()->add_parse_error(
        token->line(), std::format("An expression is expected but found '{}'.", token->lexeme()));
    move_current_left();
    return NullExpr::create_object();
}

auto Parser::function(Token* identifier) -> Expr* {
    expect_token(k_left_paren, "'(' is required after 'function' keyword."s);
    std::vector<Token*> parameters { };
    while (match_token({ k_right_paren }) == false) {
        parameters.push_back(get_token());
        match_token({ k_comma });
    }
    expect_token(k_left_brace, "'{' is required after argument list of function."s);

    FunctionExpr* function_expr { FunctionExpr::create_object(identifier, parameters, blockstmt()) };
    if (match_token({ k_left_paren })) {
        return call(function_expr);
    }
    return function_expr;
}

auto Parser::structexpr() -> Expr* {
    Token* identifier { expect_token(k_identifier, "An identifier is expected after 'struct' keyword."s) };

    expect_token(k_left_brace, "'{' is required after 'struct's identifier'."s);

    std::vector<Token*> fields { };
    std::vector<FunctionExpr*> methods { };

    struct_field_helper(fields);
    struct_method_helper(methods);

    expect_token(k_right_brace, "Each '{' must be accompanied by matching '}'."s);
    expect_token(k_semicolon, "A statement must end with ';'.");

    return StructExpr::create_object(identifier, fields, methods);
}
