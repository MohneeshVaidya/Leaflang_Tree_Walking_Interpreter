#ifndef PARSER_HPP
#define PARSER_HPP

#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <vector>

class Parser {
private:
    std::vector<const Stmt*> m_statements { };
    const std::vector<const Token*>& m_tokens { };
    uint32_t m_current { 0 };

public:
    Parser(const std::vector<const Token*>& tokens);
    ~Parser();

    // Public methods
    auto statements() const -> const std::vector<const Stmt*>&;
    auto parse() -> Parser&;

private:
    // Helper methods
    auto is_at_end() -> bool;
    auto move_current_left() -> void;
    auto move_current_right() -> void;
    auto get_token() -> const Token*;
    auto peek_token() -> const Token*;
    auto peek_prev_token() -> const Token*;
    auto match_token(std::initializer_list<TokenType> types) -> bool;
    auto match_prev_token(std::initializer_list<TokenType> types) -> bool;
    auto expect_token(TokenType type, const std::string& message) -> const Token*;
    auto synchronize() -> void;

    // Private methods
    auto top_statement() -> const Stmt*;
    auto statement() -> const Stmt*;
    auto varstmt() -> const Stmt*;
    auto conststmt() -> const Stmt*;
    auto printstmt() -> const Stmt*;
    auto printlnstmt() -> const Stmt*;
    auto expressionstmt() -> const Stmt*;
    auto blockstmt() -> const Stmt*;
    auto ifstmt() -> const Stmt*;

    auto expression() -> const Expr*;
    auto assign() -> const Expr*;
    auto ternary() -> const Expr*;
    auto or_expr() -> const Expr*;
    auto and_expr() -> const Expr*;
    auto equality() -> const Expr*;
    auto comparision() -> const Expr*;
    auto term() -> const Expr*;
    auto factor() -> const Expr*;
    auto unary() -> const Expr*;
    auto exponent() -> const Expr*;
    auto grouping() -> const Expr*;
    auto primary() -> const Expr*;
};

#endif
