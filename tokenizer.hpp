#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "token.hpp"

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

class Tokenizer {
private:
    std::string m_source { };
    std::vector<Token*> m_tokens { };
    uint32_t m_line { 1 };
    uint32_t m_current { 0 };


public:
    Tokenizer(const std::string& source);
    ~Tokenizer();

private:
    // Private methods
    auto is_at_end() -> bool;
    auto read_comment() -> void;
    auto read_number() -> std::string;
    auto read_string() -> std::string;
    auto read_identifier() -> std::string;
    auto move_current_left() -> void;
    auto move_current_right() -> void;
    auto get_character() -> char;
    auto peek_character() -> char;
    auto match_character(std::initializer_list<char> characters) -> bool;
    auto gen_token(char character) -> void;
    auto add_token(TokenType type, const std::string& lexeme) -> void;
    auto add_identifier_token(const std::string& identifier) -> void;

public:
    // Getters
    auto tokens() -> std::vector<Token*>&;

    // Public methods
    auto tokenize() -> Tokenizer&;
};

#endif
