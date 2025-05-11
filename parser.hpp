#ifndef PARSER_HPP
#define PARSER_HPP

#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <stack>
#include <vector>

class Parser {
private:
    std::vector<Stmt*> m_statements { };
    const std::vector<Token*>& m_tokens { };
    uint32_t m_current { 0 };
    std::stack<Expr*> stack { };  // Will be used to store step_statements for continue_statements in for loops.

public:
    Parser(std::vector<Token*>& tokens);
    ~Parser();

    // Public methods
    auto statements() -> std::vector<Stmt*>&;
    auto parse() -> Parser&;

private:
    // Helper methods
    auto is_at_end() -> bool;
    auto move_current_left() -> void;
    auto move_current_right() -> void;
    auto get_token() -> Token*;
    auto peek_token() -> Token*;
    auto peek_prev_token() -> Token*;
    auto peek_next_token() -> Token*;
    auto match_token(std::initializer_list<TokenType> types) -> bool;
    auto match_prev_token(std::initializer_list<TokenType> types) -> bool;
    auto match_next_token(std::initializer_list<TokenType> types) -> bool;
    auto expect_token(TokenType type, const std::string& message) -> Token*;
    auto for_helper() -> Stmt*;
    auto struct_field_helper(std::vector<Token*>& fields) -> void;
    auto struct_method_helper(std::vector<FunctionExpr*>& methods) -> void;
    auto synchronize() -> void;

    // Private methods
    auto top_statement() -> Stmt*;
    auto statement() -> Stmt*;
    auto varstmt() -> Stmt*;
    auto conststmt() -> Stmt*;
    auto printstmt() -> Stmt*;
    auto printlnstmt() -> Stmt*;
    auto expressionstmt() -> Stmt*;
    auto blockstmt() -> Stmt*;
    auto ifstmt() -> Stmt*;
    auto forstmt() -> Stmt*;
    auto breakstmt() -> Stmt*;
    auto continuestmt() -> Stmt*;
    auto returnstmt() -> Stmt*;

    auto expression() -> Expr*;
    auto assign() -> Expr*;
    auto ternary() -> Expr*;
    auto or_expr() -> Expr*;
    auto and_expr() -> Expr*;
    auto equality() -> Expr*;
    auto comparision() -> Expr*;
    auto term() -> Expr*;
    auto factor() -> Expr*;
    auto unary() -> Expr*;
    auto exponent() -> Expr*;
    auto get(Expr* left_expr) -> Expr*;
    auto call(Token* identifier) -> Expr*;
    auto call(Expr* expr) -> Expr*;
    auto grouping() -> Expr*;
    auto primary() -> Expr*;
    auto function(Token* identifier) -> Expr*;
    auto structexpr() -> Expr*;
};

#endif
