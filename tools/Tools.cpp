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
        std::string result { };

        for (char character : source) {
            result.push_back(static_cast<char>(std::toupper(character)));
        }

        return result;
    }

    std::string string_camel_to_snake(std::string_view source) {
        if (source.empty()) {
            return std::string { };
        }

        std::string result { };

        result.push_back(static_cast<char>(std::tolower(source.at(0))));
        source.remove_prefix(1);

        for (char character : source) {
            if (std::isupper(character)) {
                result.push_back('_');
                result.push_back(static_cast<char>(std::tolower(character)));
            } else {
                result.push_back(character);
            }
        }

        return result;
    }
}
