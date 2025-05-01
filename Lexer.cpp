#include "Lexer.hpp"
#include "Error.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include "tools/Tools.hpp"

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_literals;
using enum TokenType;


// Constructors
Lexer::Lexer(std::string_view source_code) :
    source_code{source_code}
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

char Lexer::peek_char() {
    return get_char(current);
}

std::string Lexer::get_number() {
    uint32_t idx{current + 1};

    std::string number{get_char(current)};

    while (std::isdigit(get_char(idx))) {
        number.push_back(source_code.at(idx++));
    }

    current = idx;
    return number;
}

std::string Lexer::get_string() {
    uint32_t idx{current};

    std::string str{};

    char character{get_char(idx++)};
    while (character != '\n' && character != '\0' && character != '"') {
        str.push_back(character);
        character = get_char(idx++);
    }

    if (character == '\n') {
        Error::get_instance()->add_error(tokens.back(), "String can not traverse multiple lines."s);
        line++;
    } else if (character == '\0') {
        Error::get_instance()->add_error(tokens.back(), "String starts but never ends."s);
    }

    current = idx;
    return str;
}

std::string Lexer::get_identifier() {
    uint32_t idx{current + 1};

    std::string identifier{get_char(current)};

    while (Tools::is_alphanumeric(get_char(idx))) {
        identifier.push_back(source_code.at(idx++));
    }

    current = idx;
    return identifier;
}

void Lexer::gen_next_token(char next_char) {
    switch (next_char) {
    case '+':
        tokens.emplace_back(k_plus, Tools::to_string(next_char), line);
        break;
    case '-':
        tokens.emplace_back(k_minus, Tools::to_string(next_char), line);
        break;
    case '*':
        if (peek_char() == '*') {
            move_current_right();
            tokens.emplace_back(k_star_star, "**"s, line);
        } else {
            tokens.emplace_back(k_star, Tools::to_string(next_char), line);
        }
        break;
    case '/':
        if (peek_char() == '/') {
            move_current_right();
            remove_comment();
        } else {
            tokens.emplace_back(k_slash, Tools::to_string(next_char), line);
        }
        break;
    case '%':
        tokens.emplace_back(k_percent, Tools::to_string(next_char), line);
        break;
    case '=':
        if (peek_char() == '=') {
            move_current_right();
            tokens.emplace_back(k_equal_equal, "=="s, line);
        } else {
            tokens.emplace_back(k_equal, Tools::to_string(next_char), line);
        }
        break;
    case '<':
        if (peek_char() == '=') {
            move_current_right();
            tokens.emplace_back(k_lesser_equal, "<="s, line);
        } else {
            tokens.emplace_back(k_lesser, Tools::to_string(next_char), line);
        }
        break;
    case '>':
        if (peek_char() == '=') {
            move_current_right();
            tokens.emplace_back(k_greater_equal, ">="s, line);
        } else {
            tokens.emplace_back(k_greater, Tools::to_string(next_char), line);
        }
        break;
    case '!':
        if (peek_char() == '=') {
            move_current_right();
            tokens.emplace_back(k_bang_equal, "!="s, line);
        } else {
            tokens.emplace_back(k_bang, Tools::to_string(next_char), line);
        }
        break;
    case '(':
        tokens.emplace_back(k_left_paren, Tools::to_string(next_char), line);
        break;
    case ')':
        tokens.emplace_back(k_right_paren, Tools::to_string(next_char), line);
        break;
    case '{':
        tokens.emplace_back(k_left_brace, Tools::to_string(next_char), line);
        break;
    case '}':
        tokens.emplace_back(k_right_brace, Tools::to_string(next_char), line);
        break;
    case ';':
        tokens.emplace_back(k_semicolon, Tools::to_string(next_char), line);
        break;
    case ',':
        tokens.emplace_back(k_comma, Tools::to_string(next_char), line);
        break;
    case '?':
        tokens.emplace_back(k_question, Tools::to_string(next_char), line);
        break;
    case ':':
        tokens.emplace_back(k_colon, Tools::to_string(next_char), line);
        break;
    case '\0':
        tokens.emplace_back(k_eof, Tools::to_string(next_char), line);
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
            std::string number{get_number()};
            tokens.emplace_back(k_number, number, number, line);

        } else if (next_char == '"') {
            std::string str{get_string()};
            tokens.emplace_back(k_string, str, str, line);

        } else if (Tools::is_alphabetical(next_char)) {
            move_current_left();
            std::string identifier{get_identifier()};
            add_identifier_token(identifier, line);
        }
        break;
    }
}

void Lexer::remove_comment() {
    uint32_t idx{current};
    char character{get_char(idx++)};
    while (character != '\n') {
        character = get_char(idx++);
    }
    current = idx;
    line++;
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
    Token token{k_identifier, identifier, line};

    if (identifier == "print") {
        token.type = k_print;
    } else if (identifier == "var") {
        token.type = k_var;
    } else if (identifier == "const") {
        token.type = k_const;
    } else if (identifier == "if") {
        token.type = k_if;
    } else if (identifier == "elseif") {
        token.type = k_elseif;
    } else if (identifier == "else") {
        token.type = k_else;
    } else if (identifier == "while") {
        token.type = k_while;
    } else if (identifier == "do") {
        token.type = k_do;
    } else if (identifier == "for") {
        token.type = k_for;
    } else if (identifier == "function") {
        token.type = k_function;
    } else if (identifier == "class") {
        token.type = k_class;
    } else if (identifier == "extends") {
        token.type = k_extends;
    } else if (identifier == "this") {
        token.type = k_this;
    } else if (identifier == "super") {
        token.type = k_super;
    } else if (identifier == "true") {
        token.type = k_true;
    } else if (identifier == "false") {
        token.type = k_false;
    } else if (identifier == "and") {
        token.type = k_and;
    } else if (identifier == "or") {
        token.type = k_or;
    } else if (identifier == "xor") {
        token.type = k_xor;
    } else if (identifier == "null") {
        token.type = k_null;
    } else {
        token.literal = identifier;
    }
    tokens.push_back(token);
}

// Public methods
const std::vector<Token> &Lexer::get_tokens() {
    char next_char{get_char()};
    while (next_char != '\0') {
        gen_next_token(next_char);
        next_char = get_char();
    }
    gen_next_token(next_char);
    return tokens;
}
