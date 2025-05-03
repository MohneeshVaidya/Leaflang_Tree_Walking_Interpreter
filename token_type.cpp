#include "token_type.hpp"

#include <ostream>

using enum TokenType;

std::ostream& operator << (std::ostream& stream, const TokenType& token) {
    switch (token) {
        case k_print        : stream << "k_print"; break;
        case k_var          : stream << "k_var"; break;
        case k_const        : stream << "k_const"; break;
        case k_if           : stream << "k_if"; break;
        case k_elseif       : stream << "k_elseif"; break;
        case k_else         : stream << "k_else"; break;
        case k_for          : stream << "k_for"; break;
        case k_do           : stream << "k_do"; break;
        case k_function     : stream << "k_function"; break;
        case k_class        : stream << "k_class"; break;
        case k_extends      : stream << "k_extends"; break;
        case k_this         : stream << "k_this"; break;
        case k_super        : stream << "k_super"; break;
        case k_true         : stream << "k_true"; break;
        case k_false        : stream << "k_false"; break;
        case k_and          : stream << "k_and"; break;
        case k_or           : stream << "k_or"; break;
        case k_xor          : stream << "k_xor"; break;
        case k_number       : stream << "k_number"; break;
        case k_string       : stream << "k_string"; break;
        case k_identifier   : stream << "k_identifier"; break;
        case k_null         : stream << "k_null"; break;
        case k_eof          : stream << "k_eof"; break;
        case k_plus         : stream << "k_plus"; break;
        case k_minus        : stream << "k_minus"; break;
        case k_slash        : stream << "k_slash"; break;
        case k_star         : stream << "k_star"; break;
        case k_star_star    : stream << "k_star_star"; break;
        case k_percent      : stream << "k_percent"; break;
        case k_equal        : stream << "k_equal"; break;
        case k_equal_equal  : stream << "k_equal_equal"; break;
        case k_bang         : stream << "k_bang"; break;
        case k_bang_equal   : stream << "k_bang_equal"; break;
        case k_lesser       : stream << "k_lesser"; break;
        case k_lesser_equal : stream << "k_lesser_equal"; break;
        case k_greater      : stream << "k_greater"; break;
        case k_greater_equal: stream << "k_greater_equal"; break;
        case k_left_paren   : stream << "k_left_paren"; break;
        case k_right_paren  : stream << "k_right_paren"; break;
        case k_left_brace   : stream << "k_left_brace"; break;
        case k_right_brace  : stream << "k_right_brace"; break;
        case k_semicolon    : stream << "k_semicolon"; break;
        case k_comma        : stream << "k_comma"; break;
        case k_question     : stream << "k_question"; break;
        case k_colon        : stream << "k_colon"; break;
        case k_dot          : stream << "k_dot"; break;
    };

    return stream;
}
