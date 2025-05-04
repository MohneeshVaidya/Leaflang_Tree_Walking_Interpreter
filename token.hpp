#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_type.hpp"

#include <cstdint>
#include <iostream>
#include <ostream>

class Token {
private:
    const TokenType m_type { };
    const std::string m_lexeme { };
    const uint32_t m_line { };

public:
    Token(TokenType type, const std::string& lexeme, uint32_t line);

    static auto create_object(TokenType type, const std::string& lexeme, uint32_t line) -> Token*;
    static auto create_object(const Token& source) -> Token*;
    static auto delete_object(const Token* object) -> void;

    // Getters
    auto type() const -> TokenType;
    auto lexeme() const -> const std::string&;
    auto line() const -> uint32_t;
};

std::ostream& operator << (std::ostream& stream, const Token& token);

#endif
