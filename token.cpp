#include "token.hpp"
#include "token_type.hpp"

#include <cstdint>
#include <ostream>
#include <string_view>

Token::Token(TokenType type, std::string_view lexeme, uint32_t line) :
    type { type },
    lexeme { lexeme },
    line { line }
    {
    }

namespace token {
auto make(TokenType type, std::string_view lexeme, uint32_t line) -> Token* {
    return new Token { type, lexeme, line };
}

auto destroy(Token* token) -> void {
    delete token;
}
};

auto operator << (std::ostream& stream, Token* token) -> std::ostream& {
    stream << "[\n";
    stream << "    type  : " << token->type << "\n";
    stream << "    lexeme: " << token->lexeme << "\n";
    stream << "    line  : " << token->line << "\n";
    stream << "]";
    return stream;
}
