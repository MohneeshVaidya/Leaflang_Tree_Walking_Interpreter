#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include <ostream>
enum class TokenType {
    // Keywords
    k_print,
    k_var,
    k_const,
    k_if,
    k_elseif,
    k_else,
    k_while,
    k_do,
    k_for,
    k_function,
    k_class,
    k_extends,
    k_and,
    k_or,
    k_xor,
    k_eof,
    k_number,
    k_string,
    k_identifier,

    // Binary operators
    k_plus,
    k_minus,
    k_slash,
    k_star,
    k_star_star,
    k_percent,
    k_equal,
    k_equal_equal,
    k_bang_equal,
    k_lesser,
    k_lesser_equal,
    k_greater,
    k_greater_equal,

    // Unary operator
    k_bang,

    // Others
    k_left_paren,
    k_right_paren,
    k_left_brace,
    k_right_brace,
    k_semicolon,
};

std::ostream& operator << (std::ostream& stream, TokenType type);

#endif
