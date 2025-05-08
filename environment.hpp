#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "leaf_object.hpp"
#include "token.hpp"

#include <string>
#include <unordered_map>

class Environment {
private:
    Environment* m_parent;

    std::unordered_map<std::string, const LeafObject*> m_var_table { };
    std::unordered_map<std::string, const LeafObject*> m_const_table { };

    Environment(Environment* parent);
    Environment(const Environment& source) = default;
    auto operator = (const Environment& other) -> Environment& = default;

    auto has_name(const std::string& name) const -> bool;
    auto assign(Environment* environment, const Token* name, const LeafObject* value) -> void;
    auto get(const Environment* environment, const Token* name) const -> const LeafObject*;
    auto get_qualifier(const Environment* environment, const Token* name) const -> std::string;

public:
    static auto create_object(Environment* parent) -> Environment*;
    static auto delete_object(const Environment* object) -> void;

    auto parent() const -> Environment*;
    auto insert_var(const Token* name, const LeafObject* value) -> void;
    auto insert_const(const Token* name, const LeafObject* value) -> void;
    auto assign(const Token* name, const LeafObject* value) -> void;
    auto get(const Token* name) const -> const LeafObject*;
    auto get_qualifier(const Token* name) const -> std::string;
};

#endif
