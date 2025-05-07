#include "environment.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"

#include <format>
#include <utility>

using namespace std::string_literals;

auto Environment::create_object() -> Environment* {
    return new Environment { };
}

auto Environment::delete_object(const Environment* object) -> void {
    delete object;
}

auto Environment::has_name(const std::string& name) const -> bool {
    return (
        m_var_table.contains(name) ||
        m_const_table.contains(name)
    );
}

auto Environment::insert_var(const Token* name, const LeafObject* value) -> void {
    if (has_name(name->lexeme())) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not re-declare variable '{}'.", name->lexeme())
        );
    }
    m_var_table.insert(std::make_pair(name->lexeme(), value));
}

auto Environment::insert_const(const Token* name, const LeafObject* value) -> void {
    if (has_name(name->lexeme())) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not re-declare variable '{}'.", name->lexeme())
        );
    }
    m_const_table.insert(std::make_pair(name->lexeme(), value));
}

auto Environment::assign(const Token* name, const LeafObject* value) -> void {
    if (has_name(name->lexeme()) == false) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not assign variable '{}' as it is not declared.", name->lexeme())
        );
    }
    m_var_table.insert_or_assign(name->lexeme(), value);
}

auto Environment::get(const Token* name) const -> const LeafObject* {
    if (has_name(name->lexeme()) == false) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not read variable '{}' as it is not declared.", name->lexeme())
        );
    }

    if (m_var_table.contains(name->lexeme())) {
        return m_var_table.at(name->lexeme());
    } else if (m_const_table.contains(name->lexeme())) {
        return m_const_table.at(name->lexeme());
    }

    return nullptr;
}

auto Environment::get_qualifier(const Token* name) const -> std::string {
    if (m_var_table.contains(name->lexeme())) {
        return "var"s;
    }

    if (m_const_table.contains(name->lexeme())) {
        return "const"s;
    }
    return ""s;
}

