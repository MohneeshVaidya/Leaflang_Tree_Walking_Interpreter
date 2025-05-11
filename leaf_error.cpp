#include "leaf_error.hpp"

#include <cstdint>
#include <format>

LeafError* LeafError::m_instance { nullptr };

auto LeafError::instance() -> LeafError* {
    if (m_instance == nullptr) {
        m_instance = new LeafError { };
    }
    return m_instance;
};

auto LeafError::delete_instance() -> void {
    delete m_instance;
    m_instance = nullptr;
}

auto LeafError::has_errors() -> bool {
    return m_has_errors;
}

auto LeafError::messages() -> std::vector<std::string>& {
    return m_messages;
}

auto LeafError::add_error(Token& token, uint32_t line, const std::string& message) -> void {
    m_has_errors = true;
    m_messages.push_back(
        std::format("Error: [At line {}] after '{}' - {}", line, token.lexeme(), message)
    );
}

auto LeafError::add_parse_error(uint32_t line, const std::string& message) -> void {
    m_has_errors = true;
    m_messages.push_back(
        std::format("ParseError: [Near line {}] - {}", line, message)
    );
}

auto LeafError::runtime_error(uint32_t line, const std::string& oper, const std::string& message) -> void {
    throw std::string {
        std::format("RuntimeError: [Near line {}] for operator '{}' - {}", line, oper, message)
    };
}

auto LeafError::runtime_error(uint32_t line, const std::string& message) -> void {
    throw std::string {
        std::format("RuntimeError: [Near line {}] - {}", line, message)
    };
}
