#include "tokenizer.hpp"
#include "leaf_error.hpp"
#include "token.hpp"
#include "token_type.hpp"
#include "tools/leaf_tools.hpp"

#include <string>
#include <vector>

using enum TokenType;

Tokenizer::Tokenizer(const std::string& source) :
    m_source { source }
    {
    }

Tokenizer::~Tokenizer() {
    for (const Token* token : m_tokens) {
        Token::delete_object(token);
    }
}


// Private methods
auto Tokenizer::is_at_end() -> bool {
    return m_current >= m_source.length();
}

auto Tokenizer::read_comment() -> void {
    char current_character { get_character() };
    while (current_character != '\n' && current_character != '\0') {
        current_character = get_character();
    }
    if (current_character == '\n') {
        m_line++;
    }
}

auto Tokenizer::read_number() -> std::string {
    std::string number { };

    char character { peek_character() };

    while (leaf::tools::is_numeric(character) || character == '.') {
        number.push_back(character);

        move_current_right();
        character = peek_character();
    }
    if (number.back() == '.') {
        number.push_back('0');
    }

    return number;
}

auto Tokenizer::read_string() -> std::string {
    std::string str { };

    bool is_special { false };
    char character { get_character() };
    while (true) {
        if (character == '"') {
            break;
        } else if (character == '\n') {
            LeafError::instance()->add_error(
                *m_tokens.back(),
                m_line++,
                "String can not traverse multiple lines."
            );
            break;
        } else if (character == '\0') {
            LeafError::instance()->add_error(
                *m_tokens.back(),
                m_line,
                "String starts but never ends (string should always end with '\"')."
            );
            break;
        }
        if (character == '\\') {
            if (is_special) {
                str.push_back(character);
                is_special = false;
            } else {
                is_special = true;
            }
        } else {
            if (is_special) {
                str.push_back(leaf::tools::get_special_character(character));
            } else {
                str.push_back(character);
            }
            is_special = false;
        }
        character = get_character();
    }

    return str;
}

auto Tokenizer::read_identifier() -> std::string {
    std::string identifier { };

    char character { peek_character() };

    while (leaf::tools::is_alphanumeric(character)) {
        identifier.push_back(character);

        move_current_right();
        character = peek_character();
    }

    return identifier;
}

auto Tokenizer::move_current_left() -> void {
    if (m_current > 0) {
        m_current--;
    }
}

auto Tokenizer::move_current_right() -> void {
    if (is_at_end() == false) {
        m_current++;
    }
}

auto Tokenizer::get_character() -> char {
    if (is_at_end() == false) {
        return m_source.at(m_current++);
    }
    return '\0';
}

auto Tokenizer::peek_character() -> char {
    if (is_at_end() == false) {
        return m_source.at(m_current);
    }
    return '\0';
}

auto Tokenizer::match_character(std::initializer_list<char> characters) -> bool {
    if (is_at_end() == false) {
        char current_character { peek_character() };
        for (char character : characters) {
            if (character == current_character) {
                move_current_right();
                return true;
            }
        }
    }
    return false;
}

auto Tokenizer::gen_token(char character) -> void {
    switch (character) {
        case '+':
            add_token(k_plus, leaf::tools::to_string(character));
            break;
        case '-':
            add_token(k_minus, leaf::tools::to_string(character));
            break;
        case '*':
            if (match_character({ '*' })) {
                add_token(k_star_star, "**");
            } else {
                add_token(k_star, leaf::tools::to_string(character));
            }
            break;
        case '/':
            if (match_character({ '/' })) {
                read_comment();
            } else {
                add_token(k_slash, leaf::tools::to_string(character));
            }
            break;
        case '%':
            add_token(k_percent, leaf::tools::to_string(character));
            break;
        case '=':
            if (match_character({ '=' })) {
                add_token(k_equal_equal, "==");
            } else {
                add_token(k_equal, leaf::tools::to_string(character));
            }
            break;
        case '!':
            if (match_character({ '=' })) {
                add_token(k_bang_equal, "!=");
            } else {
                add_token(k_bang, leaf::tools::to_string(character));
            }
            break;
        case '<':
            if (match_character({ '=' })) {
                add_token(k_lesser_equal, "<=");
            } else {
                add_token(k_lesser, leaf::tools::to_string(character));
            }
            break;
        case '>':
            if (match_character({ '=' })) {
                add_token(k_greater_equal, ">=");
            } else {
                add_token(k_greater, leaf::tools::to_string(character));
            }
            break;
        case '(':
            add_token(k_left_paren, leaf::tools::to_string(character));
            break;
        case ')':
            add_token(k_right_paren, leaf::tools::to_string(character));
            break;
        case '{':
            add_token(k_left_brace, leaf::tools::to_string(character));
            break;
        case '}':
            add_token(k_right_brace, leaf::tools::to_string(character));
            break;
        case ';':
            add_token(k_semicolon, leaf::tools::to_string(character));
            break;
        case ':':
            add_token(k_colon, leaf::tools::to_string(character));
            break;
        case ',':
            add_token(k_comma, leaf::tools::to_string(character));
            break;
        case '?':
            add_token(k_question, leaf::tools::to_string(character));
            break;
        case '.':
            add_token(k_dot, leaf::tools::to_string(character));
            break;
        case '\n':
            m_line++;
            [[fallthrough]];
        case '\r':
            [[fallthrough]];
        case '\t':
            [[fallthrough]];
        case ' ':
            break;
        case '\0':
            add_token(k_eof, "\0");
            break;
        default:
            if (leaf::tools::is_numeric(character)) {
                move_current_left();
                std::string number { read_number() };
                add_token(k_number, number);
            } else if (character == '"') {
                std::string str { read_string() };
                add_token(k_string, str);
            } else if (leaf::tools::is_alphabetic(character)) {
                move_current_left();
                std::string identifier { read_identifier() };
                add_identifier_token(identifier);
            }
            break;
    }
}

auto Tokenizer::add_token(const TokenType type, const std::string& lexeme) -> void {
    m_tokens.emplace_back(Token::create_object(type, lexeme, m_line));
}

auto Tokenizer::add_identifier_token(const std::string& identifier) -> void {
    if (identifier == "print") {
        m_tokens.push_back(Token::create_object(k_print, identifier, m_line));
    } else if (identifier == "println") {
        m_tokens.push_back(Token::create_object(k_println, identifier, m_line));
    } else if (identifier == "var") {
        m_tokens.push_back(Token::create_object(k_var, identifier, m_line));
    } else if (identifier == "const") {
        m_tokens.push_back(Token::create_object(k_const, identifier, m_line));
    } else if (identifier == "if") {
        m_tokens.push_back(Token::create_object(k_if, identifier, m_line));
    } else if (identifier == "elseif") {
        m_tokens.push_back(Token::create_object(k_elseif, identifier, m_line));
    } else if (identifier == "else") {
        m_tokens.push_back(Token::create_object(k_else, identifier, m_line));
    } else if (identifier == "for") {
        m_tokens.push_back(Token::create_object(k_for, identifier, m_line));
    } else if (identifier == "do") {
        m_tokens.push_back(Token::create_object(k_do, identifier, m_line));
    } else if (identifier == "break") {
        m_tokens.push_back(Token::create_object(k_break, identifier, m_line));
    } else if (identifier == "continue") {
        m_tokens.push_back(Token::create_object(k_continue, identifier, m_line));
    } else if (identifier == "function") {
        m_tokens.push_back(Token::create_object(k_function, identifier, m_line));
    } else if (identifier == "return") {
        m_tokens.push_back(Token::create_object(k_return, identifier, m_line));
    } else if (identifier == "struct") {
        m_tokens.push_back(Token::create_object(k_struct, identifier, m_line));
    } else if (identifier == "extends") {
        m_tokens.push_back(Token::create_object(k_extends, identifier, m_line));
    } else if (identifier == "this") {
        m_tokens.push_back(Token::create_object(k_this, identifier, m_line));
    } else if (identifier == "super") {
        m_tokens.push_back(Token::create_object(k_super, identifier, m_line));
    } else if (identifier == "true") {
        m_tokens.push_back(Token::create_object(k_true, identifier, m_line));
    } else if (identifier == "false") {
        m_tokens.push_back(Token::create_object(k_false, identifier, m_line));
    } else if (identifier == "and") {
        m_tokens.push_back(Token::create_object(k_and, identifier, m_line));
    } else if (identifier == "or") {
        m_tokens.push_back(Token::create_object(k_or, identifier, m_line));
    } else if (identifier == "xor") {
        m_tokens.push_back(Token::create_object(k_xor, identifier, m_line));
    } else if (identifier == "null") {
        m_tokens.push_back(Token::create_object(k_null, identifier, m_line));
    } else {
        m_tokens.push_back(Token::create_object(k_identifier, identifier, m_line));
    }
}


// Getters
auto Tokenizer::tokens() -> const std::vector<const Token*>& {
    return m_tokens;
}


// Public
auto Tokenizer::tokenize() -> Tokenizer& {
    char character { get_character() };

    while (character != '\0') {
        gen_token(character);
        character = get_character();
    }
    gen_token(character);

    return *this;
}
