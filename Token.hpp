#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "TokenType.hpp"

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

class Token {
public:
    TokenType type { };
    std::string lexeme { };
    std::string literal { };
    uint32_t line;

    Token(TokenType type, std::string_view lexeme, std::string_view literal, uint32_t line);
    Token(TokenType type, std::string_view lexeme, uint32_t line);
};

std::ostream& operator << (std::ostream& stream, const Token& token);

#endif
