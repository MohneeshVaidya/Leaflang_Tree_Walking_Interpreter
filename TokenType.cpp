#include "TokenType.hpp"
#include <ostream>

std::ostream& operator << (std::ostream& stream, TokenType type) {
    switch (type) {
        case TokenType::k_bang:
            stream << "bang";
            break;
        case TokenType::k_bang_equal:
            stream << "bang_equal";
            break;
        case TokenType::k_equal:
            stream << "equal";
            break;
        case TokenType::k_equal_equal:
            stream << "equal_equal";
            break;
        case TokenType::k_greater_equal:
            stream << "greater_equal";
            break;
        case TokenType::k_greater:
            stream << "greater";
            break;
        case TokenType::k_lesser:
            stream << "lesser";
            break;
        case TokenType::k_lesser_equal:
            stream << "lesser_equal";
            break;
        case TokenType::k_print:
            stream << "print";
            break;
        case TokenType::k_var:
            stream << "var";
            break;
        case TokenType::k_function:
            stream << "function";
            break;
        case TokenType::k_class:
            stream << "class";
            break;
        case TokenType::k_extends:
            stream << "extends";
            break;
        case TokenType::k_eof:
            stream << "eof";
            break;
        case TokenType::k_and:
            stream << "and";
            break;
        case TokenType::k_or:
            stream << "or";
            break;
        case TokenType::k_xor:
            stream << "xor";
            break;
        case TokenType::k_plus:
            stream << "plus";
            break;
        case TokenType::k_minus:
            stream << "minus";
            break;
        case TokenType::k_slash:
            stream << "slash";
            break;
        case TokenType::k_star:
            stream << "star";
            break;
        case TokenType::k_star_star:
            stream << "star_star";
            break;
        case TokenType::k_percent:
            stream << "percent";
            break;
        case TokenType::k_left_paren:
            stream << "left_paren";
            break;
        case TokenType::k_right_paren:
            stream << "right_paren";
            break;
        case TokenType::k_left_brace:
            stream << "left_brace";
            break;
        case TokenType::k_right_brace:
            stream << "right_brace";
            break;
        case TokenType::k_semicolon:
            stream << "semicolon";
            break;
        case TokenType::k_number:
            stream << "number";
            break;
        case TokenType::k_string:
            stream << "string";
            break;
        case TokenType::k_identifier:
            stream << "identifier";
            break;
        case TokenType::k_if:
            stream << "if";
            break;
        case TokenType::k_elseif:
            stream << "elseif";
            break;
        case TokenType::k_else:
            stream << "else";
            break;
        case TokenType::k_while:
            stream << "while";
            break;
        case TokenType::k_do:
            stream << "do";
            break;
        case TokenType::k_for:
            stream << "for";
            break;
    }
    return stream;
}
