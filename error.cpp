#include "error.hpp"
#include "token.hpp"

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct Error {
    std::vector<std::string> messages;
    bool has_errors;
};

Error error_instance { };

static auto has_errors() -> bool& {
    return error_instance.has_errors;
}

static auto messages() -> std::vector<std::string>& {
    return error_instance.messages;
}

namespace error {
auto init() -> void {
    messages() = { };
    ::has_errors() = false;
}

auto reset() -> void {
    messages().clear();
    ::has_errors() = false;
}

auto add(Token* token, std::string_view message) -> void {
    ::has_errors() = true;
    messages().push_back(
        std::format("error: [near line {}] - {}", token->line, message)
    );
}

auto add_parse_error([[maybe_unused]] TokenType type, uint32_t line, std::string_view message) -> void {
    ::has_errors() = true;
    messages().push_back(
        std::format("parse_error: [near line {}] - {}", line, message)
    );
}

auto has_errors() -> bool {
    return ::has_errors();
}

auto print() -> void {
    for (std::string_view message : messages()) {
        std::cerr << message << "\n";
    }
}
};
