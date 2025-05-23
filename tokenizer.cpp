#include "tokenizer.hpp"
#include "error.hpp"
#include "token_type.hpp"
#include "token.hpp"

#include <cctype>
#include <cstdint>
#include <format>
#include <string_view>
#include <vector>


namespace tokenizer {
using enum TokenType;

struct Tokenizer {
    std::vector<Token*> tokens { };
    std::string_view source { };
    uint32_t start { };             // points to start of current token
    uint32_t end { };               // points to end of current token
    uint32_t line { };
};

Tokenizer tokenizer { };

static auto init(std::string_view source) -> void {
    tokenizer.source = source;
    tokenizer.start  = 0;
    tokenizer.end    = 0;
    tokenizer.line   = 1;
}

static auto tokens() -> std::vector<Token*>& {
    return tokenizer.tokens;
}

static auto source() -> std::string_view {
    return tokenizer.source;
}

static auto start() -> uint32_t& {
    return tokenizer.start;
}

static auto end() -> uint32_t& {
    return tokenizer.end;
}

static auto line() -> uint32_t& {
    return tokenizer.line;
}


// helper functions
static auto is_at_end() -> char;
static auto isalphabetical(char ch) -> bool;
static auto isalphanumeric(char ch) -> bool;
static auto advance_end() -> void;
static auto get_character() -> char;
static auto peek_character() -> char;
static auto match_character(std::initializer_list<char> chars) -> bool;
static auto skipws() -> void;
static auto prepare(char ch) -> void;
static auto number() -> void;
static auto string() -> void;
static auto identifier() -> void;
static auto add_token(TokenType type) -> void;
static auto add_identifier_token() -> void;


auto get_tokens(std::string_view source) -> std::vector<Token*>& {
    init(source);

    char ch { get_character() };
    while (true) {
        if (ch == '\0') {
            break;
        }
        prepare(ch);
        ch = get_character();
    }
    add_token(TOKEN_EOF);

    return tokens();
}

auto reset() -> void {
    for (Token* curr_token : tokens()) {
        token::destroy(curr_token);
    }
    tokens().clear();
}


// helper functions implementation
static auto is_at_end() -> char {
    return end() >= source().length();
}

static auto isalphabetical(char ch) -> bool {
    return std::isalpha(ch) || (ch == '_');
}

static auto isalphanumeric(char ch) -> bool {
    return isalphabetical(ch) || std::isdigit(ch);
}

static auto advance_end() -> void {
    if (is_at_end() == false) {
        end()++;
    }
}

static auto get_character() -> char {
    if (is_at_end()) {
        return '\0';
    }
    advance_end();
    return source().at(end() - 1);
}

static auto peek_character() -> char {
    if (is_at_end()) {
        return '\0';
    }
    return source().at(end());
}

static auto match_character(std::initializer_list<char> chars) -> bool {
    for (char ch : chars) {
        if (ch == peek_character()) {
            advance_end();
            return true;
        }
    }
    return false;
}

static auto skipws() -> void {
    char ch { peek_character() };
    while (true) {
        if (std::isspace(ch) == false) {
            break;
        }
        if (ch == '\n') {
            line()++;
        }
        advance_end();
        ch = peek_character();
    }
    start() = end();
}

static auto prepare(char ch) -> void {
    if (std::isspace(ch)) {
        if (ch == '\n') {
            line()++;
        }
        return skipws();
    }
    if (std::isdigit(ch))   return number();
    if (ch == '"')          return string();
    if (isalphabetical(ch)) return identifier();

    switch (ch) {
        case '+':
            if (match_character({ '+' })) {
                add_token(TOKEN_PLUS_PLUS);
            } else {
                add_token(TOKEN_PLUS);
            }
            break;
        case '-':
            if (match_character({ '-' })) {
                add_token(TOKEN_MINUS_MINUS);
            } else {
                add_token(TOKEN_MINUS);
            }
            break;
        case '*':
            if (match_character({ '*' })) {
                add_token(TOKEN_STAR_STAR);
            } else {
                add_token(TOKEN_STAR);
            }
            break;
        case '/':
            add_token(TOKEN_SLASH);
            break;
        case '%':
            add_token(TOKEN_PERCENT);
            break;
        case '=':
            if (match_character({ '=' })) {
                add_token(TOKEN_EQUAL_EQUAL);
            } else {
                add_token(TOKEN_EQUAL);
            }
            break;
        case '!':
            if (match_character({ '=' })) {
                add_token(TOKEN_BANG_EQUAL);
            } else {
                add_token(TOKEN_BANG);
            }
            break;
        case '<':
            if (match_character({ '=' })) {
                add_token(TOKEN_LESS_EQUAL);
            } else {
                add_token(TOKEN_LESS);
            }
            break;
        case '>':
            if (match_character({ '=' })) {
                add_token(TOKEN_GREATER_EQUAL);
            } else {
                add_token(TOKEN_GREATER);
            }
            break;
        case '(':
            add_token(TOKEN_LEFT_PAREN);
            break;
        case ')':
            add_token(TOKEN_RIGHT_PAREN);
            break;
        case '{':
            add_token(TOKEN_LEFT_BRACE);
            break;
        case '}':
            add_token(TOKEN_RIGHT_BRACE);
            break;
        case '.':
            add_token(TOKEN_DOT);
            break;
        case ',':
            add_token(TOKEN_COMMA);
            break;
        case ';':
            add_token(TOKEN_SEMICOLON);
            break;
        case '?':
            add_token(TOKEN_QUESTION);
            break;
        case ':':
            add_token(TOKEN_COLON);
            break;
    }
}

static auto number() -> void {
    char ch { peek_character() };
    while (true) {
        if (ch == '.' || std::isdigit(ch)) {
            advance_end();
            ch = peek_character();
        } else {
            break;
        }
    }
    add_token(TOKEN_NUMBER);
}

static auto string() -> void {
    start() = end();
    char ch { peek_character() };
    while (true) {
        if (ch == '\0' || ch == '"') {
            break;
        }
        if (ch == '\n') {
            line()++;
            break;
        }
        advance_end();
        ch = peek_character();
    }
    if (ch == '\0' || ch == '\n') {
        error::add(
            tokens().back(),
            std::format("after token '{}' - a string must have terminating '\"'.", tokens().back()->lexeme)
        );
    }
    add_token(TOKEN_STRING);
    advance_end();
    start() = end();
}

static auto identifier() -> void {
    char ch { peek_character() };
    while (true) {
        if (isalphanumeric(ch) == false) {
            break;
        }
        advance_end();
        ch = peek_character();
    }
    add_identifier_token();
}

static auto add_token(TokenType type) -> void {
    tokens().push_back(token::make(type, source().substr(start(), end() - start()), line()));
    start() = end();
}

static auto add_identifier_token() -> void {
    std::string_view identifier { source().substr(start(), end() - start()) };

    if (identifier == "print") {
        add_token(TOKEN_PRINT);
    } else if (identifier == "println") {
        add_token(TOKEN_PRINTLN);
    } else if (identifier == "let") {
        add_token(TOKEN_LET);
    } else if (identifier == "const") {
        add_token(TOKEN_CONST);
    } else if (identifier == "if") {
        add_token(TOKEN_IF);
    } else if (identifier == "elseif") {
        add_token(TOKEN_ELSEIF);
    } else if (identifier == "else") {
        add_token(TOKEN_ELSE);
    } else if (identifier == "for") {
        add_token(TOKEN_FOR);
    } else if (identifier == "break") {
        add_token(TOKEN_BREAK);
    } else if (identifier == "continue") {
        add_token(TOKEN_CONTINUE);
    } else if (identifier == "func") {
        add_token(TOKEN_FUNC);
    } else if (identifier == "class") {
        add_token(TOKEN_CLASS);
    } else if (identifier == "extends") {
        add_token(TOKEN_EXTENDS);
    } else if (identifier == "this") {
        add_token(TOKEN_THIS);
    } else if (identifier == "super") {
        add_token(TOKEN_SUPER);
    } else if (identifier == "and") {
        add_token(TOKEN_AND);
    } else if (identifier == "or") {
        add_token(TOKEN_OR);
    } else if (identifier == "xor") {
        add_token(TOKEN_XOR);
    } else if (identifier == "true") {
        add_token(TOKEN_TRUE);
    } else if (identifier == "false") {
        add_token(TOKEN_FALSE);
    } else if (identifier == "nil") {
        add_token(TOKEN_NIL);
    } else {
        add_token(TOKEN_IDENTIFIER);
    }
}
};
