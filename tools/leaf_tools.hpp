#ifndef LEAF_TOOLS_HPP
#define LEAF_TOOLS_HPP

#include <initializer_list>
#include <optional>
#include <string>

namespace leaf::tools {
    auto to_string(char character) -> std::string;
    auto is_numeric(char character) -> bool;
    auto is_alphabetic(char character) -> bool;
    auto is_alphanumeric(char character) -> bool;
    auto string_match(const std::string& subject, std::initializer_list<std::string_view> list) -> std::optional<std::string>;
};

#endif
