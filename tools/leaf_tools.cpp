#include "leaf_tools.hpp"

#include <cctype>
#include <string>

namespace leaf::tools {
    auto get_special_character(char character) -> char {
        char result {  };
        switch (character) {
            case 't': result = '\t'; break;
            case 'n': result = '\n'; break;
            case 'r': result = '\r'; break;
        }
        return result;
    }

    auto to_string(char character) -> std::string {
        return std::string { character };
    }

    auto is_numeric(char character) -> bool {
        return std::isdigit(character);
    }

    auto is_alphabetic(char character) -> bool {
        return std::isalpha(character) || (character == '_');
    }

    auto is_alphanumeric(char character) -> bool {
        return std::isalnum(character) || (character == '_');
    }

    auto string_match(const std::string& subject, std::initializer_list<std::string_view> list) -> std::optional<std::string> {
        for (auto str : list) {
            if (str == subject) {
                return subject;
            }
        }
        return std::nullopt;
    }
};
