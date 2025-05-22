#include "token_type.hpp"
#include <ostream>

using enum TokenType;

auto operator << (std::ostream& stream, TokenType token) -> std::ostream& {
    switch (token) {
        case TOKEN_PRINT        : stream << "TOKEN_PRINT"; break;
        case TOKEN_PRINTLN      : stream << "TOKEN_PRINTLN"; break;
        case TOKEN_LET          : stream << "TOKEN_LET"; break;
        case TOKEN_CONST        : stream << "TOKEN_CONST"; break;
        case TOKEN_IF           : stream << "TOKEN_IF"; break;
        case TOKEN_ELSEIF       : stream << "TOKEN_ELSEIF"; break;
        case TOKEN_ELSE         : stream << "TOKEN_ELSE"; break;
        case TOKEN_FOR          : stream << "TOKEN_FOR"; break;
        case TOKEN_BREAK        : stream << "TOKEN_BREAK"; break;
        case TOKEN_CONTINUE     : stream << "TOKEN_CONTINUE"; break;
        case TOKEN_FUNC         : stream << "TOKEN_FUNC"; break;
        case TOKEN_CLASS        : stream << "TOKEN_CLASS"; break;
        case TOKEN_EXTENDS      : stream << "TOKEN_EXTENDS"; break;
        case TOKEN_THIS         : stream << "TOKEN_THIS"; break;
        case TOKEN_SUPER        : stream << "TOKEN_SUPER"; break;
        case TOKEN_AND          : stream << "TOKEN_AND"; break;
        case TOKEN_OR           : stream << "TOKEN_OR"; break;
        case TOKEN_XOR          : stream << "TOKEN_XOR"; break;
        case TOKEN_TRUE         : stream << "TOKEN_TRUE"; break;
        case TOKEN_FALSE        : stream << "TOKEN_FALSE"; break;
        case TOKEN_NIL          : stream << "TOKEN_NIL"; break;
        case TOKEN_IDENTIFIER   : stream << "TOKEN_IDENTIFIER"; break;
        case TOKEN_STRING       : stream << "TOKEN_STRING"; break;
        case TOKEN_NUMBER       : stream << "TOKEN_NUMBER"; break;
        case TOKEN_PLUS         : stream << "TOKEN_PLUS"; break;
        case TOKEN_PLUS_PLUS    : stream << "TOKEN_PLUS_PLUS"; break;
        case TOKEN_MINUS        : stream << "TOKEN_MINUS"; break;
        case TOKEN_MINUS_MINUS  : stream << "TOKEN_MINUS_MINUS"; break;
        case TOKEN_SLASH        : stream << "TOKEN_SLASH"; break;
        case TOKEN_STAR         : stream << "TOKEN_STAR"; break;
        case TOKEN_STAR_STAR    : stream << "TOKEN_STAR_STAR"; break;
        case TOKEN_PERCENT      : stream << "TOKEN_PERCENT"; break;
        case TOKEN_EQUAL        : stream << "TOKEN_EQUAL"; break;
        case TOKEN_EQUAL_EQUAL  : stream << "TOKEN_EQUAL_EQUAL"; break;
        case TOKEN_BANG         : stream << "TOKEN_BANG"; break;
        case TOKEN_BANG_EQUAL   : stream << "TOKEN_BANG_EQUAL"; break;
        case TOKEN_LESS         : stream << "TOKEN_LESS"; break;
        case TOKEN_LESS_EQUAL   : stream << "TOKEN_LESS_EQUAL"; break;
        case TOKEN_GREATER      : stream << "TOKEN_GREATER"; break;
        case TOKEN_GREATER_EQUAL: stream << "TOKEN_GREATER_EQUAL"; break;
        case TOKEN_QUESTION     : stream << "TOKEN_QUESTION"; break;
        case TOKEN_COLON        : stream << "TOKEN_COLON"; break;
        case TOKEN_DOT          : stream << "TOKEN_DOT"; break;
        case TOKEN_SEMICOLON    : stream << "TOKEN_SEMICOLON"; break;
        case TOKEN_COMMA        : stream << "TOKEN_COMMA"; break;
        case TOKEN_LEFT_PAREN   : stream << "TOKEN_LEFT_PAREN"; break;
        case TOKEN_RIGHT_PAREN  : stream << "TOKEN_RIGHT_PAREN"; break;
        case TOKEN_LEFT_BRACE   : stream << "TOKEN_LEFT_BRACE"; break;
        case TOKEN_RIGHT_BRACE  : stream << "TOKEN_RIGHT_BRACE"; break;
        case TOKEN_EOF          : stream << "TOKEN_EOF"; break;
    }
    return stream;
}
