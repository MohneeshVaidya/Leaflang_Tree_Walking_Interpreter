#include "Tools.hpp"

#include <cctype>
#include <string>

namespace Tools {
    std::string to_string(char character) {
        return std::string { character };
    }

    bool is_alphanumeric(char character) {
        return std::isalnum(character) || character == '_';
    }

    bool is_alphabetical(char character) {
        return std::isalpha(character) || character == '_';
    }
}
