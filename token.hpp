#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_type.hpp"

#include <cstdint>
#include <iostream>
#include <ostream>

class Token {
private:
    TokenType m_type { };
    std::string m_lexeme { };
    uint32_t m_line { };

public:
    Token(TokenType type, const std::string& lexeme, uint32_t line);

    static auto create_object(TokenType type, const std::string& lexeme, uint32_t line) -> Token*;
    static auto create_object(Token& source) -> Token*;
    static auto delete_object(Token* object) -> void;

    // Getters
    auto type() -> TokenType;
    auto lexeme() -> const std::string&;
    auto line() -> uint32_t;
};

std::ostream& operator << (std::ostream& stream, Token& token);

#endif
