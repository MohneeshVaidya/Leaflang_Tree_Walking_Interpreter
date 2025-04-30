#include "Lexer.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include "tools/Tools.hpp"

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

// Constructors
Lexer::Lexer(std::string_view source_code):
    source_code { source_code }
    {
    }

// Private methods
char Lexer::get_char() {
    if (current < source_code.length()) {
        return source_code.at(current++);
    }
    return '\0';
}

char Lexer::get_char(uint32_t idx) {
    if (idx < source_code.length()) {
        return source_code.at(idx);
    }
    return '\0';
}

std::string Lexer::get_number() {
    uint32_t idx   { current + 1 };

    std::string number { get_char(current) };

    while (std::isdigit(get_char(idx))) {
        number.push_back(source_code.at(idx++));
    }

    current = idx;
    return number;
}

std::string Lexer::get_string() {
    uint32_t idx { current };

    std::string str { };

    char character { get_char(idx++) };
    while (character != '\n' && character != '\0' && character != '"') {
        str.push_back(character);
        character = get_char(idx++);
    }

    if (character == '\n' || character == '\0') {
        // Error: string doesn't have trailing '"'.
    }

    current = idx;
    return str;
}

std::string Lexer::get_identifier() {
    uint32_t idx { current + 1 };

    std::string identifier { get_char(current) };

    while (Tools::is_alphanumeric(get_char(idx))) {
        identifier.push_back(source_code.at(idx++));
    }

    current = idx;
    return identifier;
}

void Lexer::gen_next_token(char next_char) {
    switch (next_char) {
        case '+':
            tokens.emplace_back(TokenType::k_plus, Tools::to_string(next_char), line);
            break;
        case '-':
            tokens.emplace_back(TokenType::k_minus, Tools::to_string(next_char), line);
            break;
        case '*':
            tokens.emplace_back(TokenType::k_star, Tools::to_string(next_char), line);
            break;
        case '/':
            tokens.emplace_back(TokenType::k_slash, Tools::to_string(next_char), line);
            break;
        case '%':
            tokens.emplace_back(TokenType::k_percent, Tools::to_string(next_char), line);
            break;
        case '=':
            tokens.emplace_back(TokenType::k_equal, Tools::to_string(next_char), line);
            break;
        case '<':
            tokens.emplace_back(TokenType::k_lesser, Tools::to_string(next_char), line);
            break;
        case '>':
            tokens.emplace_back(TokenType::k_greater, Tools::to_string(next_char), line);
            break;
        case '!':
            tokens.emplace_back(TokenType::k_bang, Tools::to_string(next_char), line);
            break;
        case '(':
            tokens.emplace_back(TokenType::k_left_paren, Tools::to_string(next_char), line);
            break;
        case ')':
            tokens.emplace_back(TokenType::k_right_paren, Tools::to_string(next_char), line);
            break;
        case '{':
            tokens.emplace_back(TokenType::k_left_brace, Tools::to_string(next_char), line);
            break;
        case '}':
            tokens.emplace_back(TokenType::k_right_brace, Tools::to_string(next_char), line);
            break;
        case ';':
            tokens.emplace_back(TokenType::k_semicolon, Tools::to_string(next_char), line);
            break;
        case '\0':
            tokens.emplace_back(TokenType::k_eof, Tools::to_string(next_char), line);
            break;
        case '\n':
            line++;
            [[fallthrough]];
        case ' ':
            [[fallthrough]];
        case '\t':
            [[fallthrough]];
        case '\r':
            break;
        default:
            if (std::isdigit(next_char)) {
                move_current_left();
                std::string number { get_number() };
                tokens.emplace_back(TokenType::k_number, number, number, line);

            } else if (next_char == '"') {
                std::string str { get_string() };
                tokens.emplace_back(TokenType::k_string, str, str, line);
            } else if (Tools::is_alphabetical(next_char)) {
                move_current_left();
                std::string identifier { get_identifier() };
                add_identifier_token(identifier, line);
            }
            break;
    }
}

void Lexer::move_current_left() {
    if (current > 0) {
        current--;
    }
}

void Lexer::move_current_right() {
    if (current < source_code.length()) {
        current++;
    }
}

void Lexer::add_identifier_token(std::string_view identifier, uint32_t line) {
    Token token { TokenType::k_identifier, identifier, line };

    if (identifier == "print") {
        token.type = TokenType::k_print;
    } else if (identifier == "var") {
        token.type = TokenType::k_var;
    } else if (identifier == "const") {
        token.type = TokenType::k_const;
    } else if (identifier == "if") {
        token.type = TokenType::k_if;
    } else if (identifier == "elseif") {
        token.type = TokenType::k_elseif;
    } else if (identifier == "else") {
        token.type = TokenType::k_else;
    } else if (identifier == "while") {
        token.type = TokenType::k_while;
    } else if (identifier == "do") {
        token.type = TokenType::k_do;
    } else if (identifier == "for") {
        token.type = TokenType::k_for;
    } else if (identifier == "function") {
        token.type = TokenType::k_function;
    } else if (identifier == "class") {
        token.type = TokenType::k_class;
    } else if (identifier == "extends") {
        token.type = TokenType::k_extends;
    } else if (identifier == "and") {
        token.type = TokenType::k_and;
    } else if (identifier == "or") {
        token.type = TokenType::k_or;
    } else if (identifier == "xor") {
        token.type = TokenType::k_xor;
    } else {
        token.literal = identifier;
    }
    tokens.push_back(token);
}

// Public methods
const std::vector<Token>& Lexer::get_tokens() {
    char next_char { get_char() };
    while (next_char != '\0') {
        gen_next_token(next_char);
        next_char = get_char();
    }
    gen_next_token(next_char);
    return tokens;
}
