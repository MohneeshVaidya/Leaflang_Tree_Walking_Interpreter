#include "Error.hpp"

#include <format>

// Initializing static property
Error* Error::instance { nullptr };

// Public methods
Error::~Error() {
    delete this;
}

Error* Error::get_instance() {
    if (instance == nullptr) {
        instance = new Error { };
    }
    return instance;
}

void Error::add_error(const Token& token, std::string_view message) {
    has_errors = true;
    errors.push_back(
        std::format("Error: [line {}] after '{}' - {}", token.line, token.lexeme, message)
    );
}
