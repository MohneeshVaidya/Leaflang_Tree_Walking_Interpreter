#include "environment.hpp"
#include "leaf_error.hpp"
#include "leaf_object.hpp"

#include <format>
#include <utility>

using namespace std::string_literals;

auto Environment::create_object() -> Environment* {
    return new Environment { };
}

auto Environment::create_object(Environment* parent) -> Environment* {
    return new Environment { parent };
}

auto Environment::delete_object(const Environment* object) -> void {
    if (object->m_is_closure == false) {
        delete object;
    }
}

auto Environment::get_closure(const Environment* environment) -> Environment* {
    const Environment* curr_environment { environment };
    Environment* closure { create_object() };

    while (curr_environment) {
        for (auto item : curr_environment->m_var_table) {
            if (closure->m_var_table.contains(item.first) == false) {
                closure->m_var_table.insert_or_assign(item.first, item.second);
            }
        }
        for (auto item : curr_environment->m_const_table) {
            if (closure->m_const_table.contains(item.first) == false) {
                closure->m_const_table.insert_or_assign(item.first, item.second);
            }
        }
        curr_environment = curr_environment->parent();
    }
    return closure;
}

Environment::Environment(Environment* parent) :
    m_parent { parent }
    {
    }

auto Environment::has_name(const std::string& name) const -> bool {
    return (
        m_var_table.contains(name) ||
        m_const_table.contains(name)
    );
}

auto Environment::assign(Environment* environment, const Token* name, const LeafObject* value) -> void {
    if (environment->m_var_table.contains(name->lexeme())) {
        environment->m_var_table.insert_or_assign(name->lexeme(), value);
        return;
    }

    if (environment->m_const_table.contains(name->lexeme())) {
        environment->m_const_table.insert_or_assign(name->lexeme(), value);
        return;
    }

    if (environment->m_parent) {
        assign(environment->m_parent, name, value);
        return;
    }

    LeafError::instance()->runtime_error(
        name->line(),
        std::format("You can not assign variable '{}' as it is not declared.", name->lexeme())
    );
}

auto Environment::get(const Environment* environment, const Token* name) const -> const LeafObject* {
    if (environment->m_var_table.contains(name->lexeme())) {
        return environment->m_var_table.at(name->lexeme());
    }

    if (environment->m_const_table.contains(name->lexeme())) {
        return environment->m_const_table.at(name->lexeme());
    }

    if (environment->m_parent) {
        return get(environment->m_parent, name);
    }

    LeafError::instance()->runtime_error(
        name->line(),
        std::format("You can not read variable '{}' as it is not declared.", name->lexeme())
    );

    return nullptr;
}

auto Environment::get_qualifier(const Environment* environment, const Token* name) const -> std::string {
    if (environment->m_var_table.contains(name->lexeme())) {
        return "var"s;
    }

    if (environment->m_const_table.contains(name->lexeme())) {
        return "const"s;
    }

    if (environment->m_parent) {
        return get_qualifier(environment->m_parent, name);
    }

    return ""s;
}


auto Environment::parent() const -> Environment* {
    return m_parent;
}

auto Environment::set_parent(Environment* parent) -> void {
    m_parent = parent;
}

auto Environment::make_closure() -> Environment* {
    m_is_closure = true;
    return this;
}

auto Environment::insert_var(const Token* name, const LeafObject* value) -> void {
    if (has_name(name->lexeme())) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not re-declare variable '{}' in same scope.", name->lexeme())
        );
    }
    m_var_table.insert(std::make_pair(name->lexeme(), value));
}

auto Environment::insert_const(const Token* name, const LeafObject* value) -> void {
    if (has_name(name->lexeme())) {
        LeafError::instance()->runtime_error(
            name->line(),
            std::format("You can not re-declare variable '{}' in same scope.", name->lexeme())
        );
    }
    m_const_table.insert(std::make_pair(name->lexeme(), value));
}

auto Environment::assign(const Token* name, const LeafObject* value) -> void {
    assign(this, name, value);
}

auto Environment::get(const Token* name) const -> const LeafObject* {
    return get(this, name);
}

auto Environment::get_qualifier(const Token* name) const -> std::string {
    return get_qualifier(this, name);
}

