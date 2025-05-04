#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <iostream>

Token::Token(TokenType type, const std::string& lexeme, uint32_t line) :
    m_type { type },
    m_lexeme { lexeme },
    m_line { line }
    {
    }

auto Token::create_object(TokenType type, const std::string& lexeme, uint32_t line) -> Token* {
    return new Token { type, lexeme, line };
}

auto Token::create_object(const Token& source) -> Token* {
    return create_object(source.type(), source.lexeme(), source.line());
}

auto Token::delete_object(const Token* object) -> void {
    delete object;
    object = nullptr;
}

// Getters

auto Token::type() const -> TokenType {
    return m_type;
}

auto Token::lexeme() const -> const std::string& {
    return m_lexeme;
}

auto Token::line() const -> uint32_t {
    return m_line;
}

std::ostream& operator << (std::ostream& stream, const Token& token) {
    stream << "{\n";
    stream << "    type   : " << token.type() << "\n";
    stream << "    lexeme : " << token.lexeme() << "\n";
    stream << "    line   : " << token.line() << "\n";
    stream << "}";
    return stream;
}
