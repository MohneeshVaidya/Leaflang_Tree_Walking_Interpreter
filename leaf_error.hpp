#ifndef LEAF_ERROR_HPP
#define LEAF_ERROR_HPP

#include "token.hpp"

#include <cstdint>
#include <string>
#include <vector>

class LeafError {
private:
    static LeafError* m_instance;

    LeafError() = default;

public:
    static auto instance() -> LeafError*;
    static auto delete_instance() -> void;

private:
    bool m_has_errors { false };
    std::vector<std::string> m_messages{ };

public:
    auto has_errors() const -> bool;
    auto messages() const -> const std::vector<std::string>&;

    auto add_error(const Token& token, const uint32_t line, const std::string& message) -> void;
    auto add_parse_error(const uint32_t line, const std::string& message) -> void;
    auto runtime_error(const uint32_t line, const std::string& oper, const std::string& message) -> void;
};

#endif
