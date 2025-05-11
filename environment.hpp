#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "leaf_object.hpp"
#include "token.hpp"

#include <string>
#include <unordered_map>

// Forwared declaration
class LeafFunction;

class Environment {
private:
    Environment* m_parent { nullptr };
    bool m_is_closure { false };

    std::unordered_map<std::string, LeafObject*> m_var_table { };
    std::unordered_map<std::string, LeafObject*> m_const_table { };
    std::unordered_map<std::string,
        std::unordered_map<std::string, LeafFunction*>> m_metaclass { };

    Environment() = default;
    Environment(Environment* parent);
    Environment(Environment& source) = default;
    auto operator = (Environment& other) -> Environment& = default;

    auto has_name(const std::string& name) -> bool;
    auto assign(Environment* environment, Token* name, LeafObject* value) -> void;
    auto get(Environment* environment, Token* name) -> LeafObject*;
    auto get_qualifier(Environment* environment, Token* name) -> std::string;

public:
    static auto create_object() -> Environment*;
    static auto create_object(Environment* parent) -> Environment*;
    static auto delete_object(Environment* object) -> void;

    static auto get_closure(Environment* environment) -> Environment*;

    auto parent() -> Environment*;
    auto set_parent(Environment* parent) -> void;
    auto make_closure() -> Environment*;
    auto insert_var(Token* name, LeafObject* value) -> void;
    auto insert_const(Token* name, LeafObject* value) -> void;
    auto assign(Token* name, LeafObject* value) -> void;
    auto get(Token* name) -> LeafObject*;
    auto get_qualifier(Token* name) -> std::string;

    auto metaclass_add_method(Token* struct_name, const std::string& method_name, LeafFunction* method) -> void;
    auto metaclass_get_method(Token* struct_name, const std::string& method_name) -> LeafFunction*;
};

#endif
