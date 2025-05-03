#include "leaf_error.hpp"

#include <format>

LeafError* LeafError::m_instance { nullptr };

auto LeafError::instance() -> LeafError* {
    if (m_instance == nullptr) {
        m_instance = new LeafError { };
    }
    return m_instance;
};

auto LeafError::destroy_instance() -> void {
    delete m_instance;
    m_instance = nullptr;
}

auto LeafError::add_error(const Token& token, const uint32_t line, const std::string& message) -> void {
    m_has_errors = true;
    m_messages.push_back(
        std::format("Error: [At line {}] after '{}' - {}", line, token.lexeme(), message)
    );
}

auto LeafError::has_errors() const -> bool {
    return m_has_errors;
}

auto LeafError::messages() const -> const std::vector<std::string>& {
    return m_messages;
}
