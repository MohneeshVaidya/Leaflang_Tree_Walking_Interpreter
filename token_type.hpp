#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <ostream>

enum class TokenType {
    // Keywords
    k_print,
    k_println,
    k_var,
    k_const,
    k_if,
    k_elseif,
    k_else,
    k_for,
    k_do,
    k_function,
    k_class,
    k_extends,
    k_this,
    k_super,
    k_true,
    k_false,
    k_and,
    k_or,
    k_xor,
    k_number,
    k_string,
    k_identifier,
    k_null,
    k_eof,

    // Operators
    k_plus,
    k_minus,
    k_slash,
    k_star,
    k_star_star,
    k_percent,
    k_equal,
    k_equal_equal,
    k_bang,
    k_bang_equal,
    k_lesser,
    k_lesser_equal,
    k_greater,
    k_greater_equal,

    // Punctuations
    k_left_paren,
    k_right_paren,
    k_left_brace,
    k_right_brace,
    k_semicolon,
    k_comma,
    k_question,
    k_colon,
    k_dot,
};

std::ostream& operator << (std::ostream& stream, const TokenType& type);

#endif
