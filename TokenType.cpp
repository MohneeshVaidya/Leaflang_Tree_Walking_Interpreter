#include "TokenType.hpp"
#include <ostream>

using enum TokenType;

std::ostream& operator << (std::ostream& stream, TokenType type) {
    switch (type) {
        case k_bang:
            stream << "bang";
            break;
        case k_bang_equal:
            stream << "bang_equal";
            break;
        case k_equal:
            stream << "equal";
            break;
        case k_equal_equal:
            stream << "equal_equal";
            break;
        case k_greater_equal:
            stream << "greater_equal";
            break;
        case k_greater:
            stream << "greater";
            break;
        case k_lesser:
            stream << "lesser";
            break;
        case k_lesser_equal:
            stream << "lesser_equal";
            break;
        case k_print:
            stream << "print";
            break;
        case k_var:
            stream << "var";
            break;
        case k_const:
            stream << "const";
            break;
        case k_function:
            stream << "function";
            break;
        case k_class:
            stream << "class";
            break;
        case k_extends:
            stream << "extends";
            break;
        case k_eof:
            stream << "eof";
            break;
        case k_and:
            stream << "and";
            break;
        case k_or:
            stream << "or";
            break;
        case k_xor:
            stream << "xor";
            break;
        case k_plus:
            stream << "plus";
            break;
        case k_minus:
            stream << "minus";
            break;
        case k_slash:
            stream << "slash";
            break;
        case k_star:
            stream << "star";
            break;
        case k_star_star:
            stream << "star_star";
            break;
        case k_percent:
            stream << "percent";
            break;
        case k_left_paren:
            stream << "left_paren";
            break;
        case k_right_paren:
            stream << "right_paren";
            break;
        case k_left_brace:
            stream << "left_brace";
            break;
        case k_right_brace:
            stream << "right_brace";
            break;
        case k_semicolon:
            stream << "semicolon";
            break;
        case k_comma:
            stream << "comma";
            break;
        case k_number:
            stream << "number";
            break;
        case k_string:
            stream << "string";
            break;
        case k_identifier:
            stream << "identifier";
            break;
        case k_if:
            stream << "if";
            break;
        case k_elseif:
            stream << "elseif";
            break;
        case k_else:
            stream << "else";
            break;
        case k_while:
            stream << "while";
            break;
        case k_do:
            stream << "do";
            break;
        case k_for:
            stream << "for";
            break;
    }
    return stream;
}
