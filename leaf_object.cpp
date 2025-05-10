#include "leaf_object.hpp"

using enum ObjectType;


// LeafObject
auto LeafObject::delete_object(const LeafObject* object) -> void {
    delete object;
}

LeafObject::LeafObject(const Token* token) :
    m_token { token == nullptr ? nullptr : Token::create_object(*token) }
    {
    }

LeafObject::~LeafObject() {
    Token::delete_object(m_token);
}

auto operator << (std::ostream& stream, const LeafObject* object) -> std::ostream& {
    if (object->type() == k_null) {
        stream << "null";
    } else if (object->type() == k_number) {
        stream << dynamic_cast<const LeafNumber*>(object)->value();
    } else if (object->type() == k_string) {
        stream << dynamic_cast<const LeafString*>(object)->value();
    } else if (object->type() == k_bool) {
        stream << dynamic_cast<const LeafBool*>(object)->value();
    } else if (object->type() == k_function) {
        stream << "leaf_function { }";
    }
    return stream;
}


// LeafNull
auto LeafNull::create_object(const Token* token) -> LeafNull* {
    return new LeafNull { token };
}

auto LeafNull::create_object() -> LeafNull* {
    return new LeafNull { };
}

LeafNull::LeafNull(const Token* token) :
    LeafObject { token }
    {
    }

LeafNull::LeafNull() :
    LeafObject { nullptr }
    {
    }

auto LeafNull::type() const -> ObjectType {
    return k_null;
}


auto LeafNull::is_truthy() const -> bool {
    return false;
}


// LeafNumber
auto LeafNumber::create_object(const Token* token) -> LeafNumber* {
    return new LeafNumber { token };
}

auto LeafNumber::create_object(const double value) -> LeafNumber* {
    return new LeafNumber { value };
}

LeafNumber::LeafNumber(const Token* token) :
    LeafObject { token },
    m_value { std::atof(token->lexeme().data()) }
    {
    }

LeafNumber::LeafNumber(const double value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafNumber::type() const -> ObjectType {
    return k_number;
}

auto LeafNumber::is_truthy() const -> bool {
    return m_value != 0;
}

auto LeafNumber::value() const -> double {
    return m_value;
}


// LeafString
auto LeafString::create_object(const Token* token) -> LeafString* {
    return new LeafString { token };
}

auto LeafString::create_object(const std::string& value) -> LeafString* {
    return new LeafString { value };
}

LeafString::LeafString(const Token* token) :
    LeafObject { token },
    m_value { token->lexeme() }
    {
    }

LeafString::LeafString(const std::string& value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafString::type() const -> ObjectType {
    return k_string;
}

auto LeafString::is_truthy() const -> bool {
    return m_value.empty() == false;
}

auto LeafString::value() const -> const std::string& {
    return m_value;
}


// LeafBool
auto LeafBool::create_object(const Token* token) -> LeafBool* {
    return new LeafBool { token };
}

auto LeafBool::create_object(const bool value) -> LeafBool* {
    return new LeafBool { value };
}

LeafBool::LeafBool(const Token* token) :
    LeafObject { token },
    m_value { token->lexeme() == "true" }
    {
    }

LeafBool::LeafBool(const bool value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafBool::type() const -> ObjectType {
    return k_bool;
}

auto LeafBool::is_truthy() const -> bool {
    return m_value;
}

auto LeafBool::value() const -> bool {
    return m_value;
}
