#ifndef leaf_error_hpp
#define leaf_error_hpp

#include "token.hpp"

#include <string_view>

namespace error {
enum class ErrorType {
    ERROR_PARSE
};

auto init() -> void;
auto reset() -> void;
auto add(Token* token, std::string_view message) -> void;
auto add_parse_error(TokenType type, uint32_t line, std::string_view message) -> void;
auto has_errors() -> bool;
auto print() -> void;
};

#endif
