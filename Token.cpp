#include "Token.hpp"

#include <ostream>

Token::Token(TokenType type, std::string_view lexeme, std::string_view literal, uint32_t line):
    type { type },
    lexeme { lexeme },
    literal { literal },
    line { line }
    {
    }

Token::Token(TokenType type, std::string_view lexeme, uint32_t line):
    Token { type, lexeme, "", line }
    {
    }

std::ostream& operator << (std::ostream& stream, const Token& token) {
    // stream << token.type << " " << token.lexeme << " " << token.literal << " " << token.line;
    stream << "{\n";
    stream << "    token  : " << token.type << "\n";
    stream << "    lexeme : " << token.lexeme << "\n";
    stream << "    literal: " << token.literal << "\n";
    stream << "    line   : " << token.line << "\n";
    stream << "}";
    return stream;
}
