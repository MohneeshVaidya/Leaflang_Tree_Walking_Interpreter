#ifndef leaf_token_hpp
#define leaf_token_hpp

#include "token_type.hpp"

#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>

struct Token {
    TokenType type;
    std::string lexeme;
    uint32_t line;

    Token(TokenType type, std::string_view lexeme, uint32_t line);
};

namespace token {
auto make(TokenType type, std::string_view lexeme, uint32_t line) -> Token*;
auto destroy(Token* token) -> void;
};

auto operator << (std::ostream& stream, Token* token) -> std::ostream&;

#endif
