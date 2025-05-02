#include "Tools.hpp"

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

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

    std::vector<std::string> string_split(std::string_view source, std::string_view delims) {
        std::vector<std::string> parts { };

        std::string part { };

        for (uint32_t i { 0 }; i < source.length(); i++) {
            if (delims.find(source.at(i)) == std::string::npos) {
                part.push_back(source.at(i));
            } else {
                parts.push_back(part);
                part.clear();

                i++;
                while (i < source.length() && std::isspace(source.at(i))) {
                    i++;
                }
                i--;
            }
        }

        if (part.empty() == false) {
            parts.push_back(part);
        }

        return parts;
    }

    std::string string_toupper(std::string_view source) {
        int32_t diff { 'a' - 'A' };

        std::string result { };

        for (char character : source) {
            if ('a' <= character && character <= 'z') {
                result.push_back(static_cast<char>(character - diff));
            } else {
                result.push_back(character);
            }
        }

        return result;
    }
}
