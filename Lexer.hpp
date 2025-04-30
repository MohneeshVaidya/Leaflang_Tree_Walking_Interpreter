#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

class Lexer {
public:
    std::string_view source_code { };
    std::vector<Token> tokens { };
    uint32_t current { 0 };
    uint32_t line { 1 };

    // Constructors
    Lexer(std::string_view source_code);

private:
    // Private methods
    char get_char();
    char get_char(uint32_t idx);
    std::string get_number();
    std::string get_string();
    std::string get_identifier();
    void gen_next_token(char next_char);
    void move_current_left();
    void move_current_right();
    void add_identifier_token(std::string_view identifier, uint32_t line);


public:
    // Public methdos
    const std::vector<Token>& get_tokens();
};

#endif
