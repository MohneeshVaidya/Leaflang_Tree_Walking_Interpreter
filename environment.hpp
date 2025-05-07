#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "leaf_object.hpp"
#include "token.hpp"

#include <string>
#include <unordered_map>

class Environment {
private:
    std::unordered_map<std::string, const LeafObject*> m_var_table { };
    std::unordered_map<std::string, const LeafObject*> m_const_table { };

    Environment() = default;

    auto has_name(const std::string& name) const -> bool;

public:
    static auto create_object() -> Environment*;
    static auto delete_object(const Environment* object) -> void;

    auto insert_var(const Token* name, const LeafObject* value) -> void;
    auto insert_const(const Token* name, const LeafObject* value) -> void;
    auto assign(const Token* name, const LeafObject* value) -> void;
    auto get(const Token* name) const -> const LeafObject*;
    auto get_qualifier(const Token* name) const -> std::string;
};

#endif
