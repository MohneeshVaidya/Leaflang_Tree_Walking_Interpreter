#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <string_view>
#include <vector>

namespace Tools {
    std::string to_string(char character);
    bool is_alphanumeric(char character);
    bool is_alphabetical(char character);
    std::vector<std::string> string_split(std::string_view source, std::string_view delims);
    std::string string_toupper(std::string_view source);
};

#endif
