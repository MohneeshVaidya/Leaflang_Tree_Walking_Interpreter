#include "leaf_object.hpp"
#include "leaf_struct.hpp"

#include <iomanip>
#include <ios>

using enum ObjectType;


// LeafObject
auto LeafObject::delete_object(LeafObject* object) -> void {
    delete object;
}

LeafObject::LeafObject(Token* token) :
    m_token { token == nullptr ? nullptr : Token::create_object(*token) }
    {
    }

LeafObject::~LeafObject() {
    Token::delete_object(m_token);
}

auto operator << (std::ostream& stream, LeafObject* object) -> std::ostream& {
    if (object->type() == k_null) {
        stream << "null";
    } else if (object->type() == k_number) {
        stream << dynamic_cast<LeafNumber*>(object)->value();
    } else if (object->type() == k_string) {
        stream << dynamic_cast<LeafString*>(object)->value();
    } else if (object->type() == k_bool) {
        stream << dynamic_cast<LeafBool*>(object)->value();
    } else if (object->type() == k_function) {
        stream << "leaf_function { }";
    } else if (object->type() == k_struct_instance) {
        LeafStructInstance* struct_instance { dynamic_cast<LeafStructInstance*>(object) };
        stream << struct_instance->struct_name()->lexeme() << " {\n";
        stream << std::left;
        for (auto item : struct_instance->value()) {
            stream << "    ";
            stream << std::setw(12) << item.first << " : " << item.second << "\n";
        }
        stream << std::right;
        stream << "}";
    }
    return stream;
}


// LeafNull
auto LeafNull::create_object(Token* token) -> LeafNull* {
    return new LeafNull { token };
}

auto LeafNull::create_object() -> LeafNull* {
    return new LeafNull { };
}

LeafNull::LeafNull(Token* token) :
    LeafObject { token }
    {
    }

LeafNull::LeafNull() :
    LeafObject { nullptr }
    {
    }

auto LeafNull::type() -> ObjectType {
    return k_null;
}


auto LeafNull::is_truthy() -> bool {
    return false;
}


// LeafNumber
auto LeafNumber::create_object(Token* token) -> LeafNumber* {
    return new LeafNumber { token };
}

auto LeafNumber::create_object(double value) -> LeafNumber* {
    return new LeafNumber { value };
}

LeafNumber::LeafNumber(Token* token) :
    LeafObject { token },
    m_value { std::atof(token->lexeme().data()) }
    {
    }

LeafNumber::LeafNumber(double value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafNumber::type() -> ObjectType {
    return k_number;
}

auto LeafNumber::is_truthy() -> bool {
    return m_value != 0;
}

auto LeafNumber::value() -> double {
    return m_value;
}


// LeafString
auto LeafString::create_object(Token* token) -> LeafString* {
    return new LeafString { token };
}

auto LeafString::create_object(const std::string& value) -> LeafString* {
    return new LeafString { value };
}

LeafString::LeafString(Token* token) :
    LeafObject { token },
    m_value { token->lexeme() }
    {
    }

LeafString::LeafString(const std::string& value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafString::type() -> ObjectType {
    return k_string;
}

auto LeafString::is_truthy() -> bool {
    return m_value.empty() == false;
}

auto LeafString::value() -> std::string& {
    return m_value;
}


// LeafBool
auto LeafBool::create_object(Token* token) -> LeafBool* {
    return new LeafBool { token };
}

auto LeafBool::create_object(bool value) -> LeafBool* {
    return new LeafBool { value };
}

LeafBool::LeafBool(Token* token) :
    LeafObject { token },
    m_value { token->lexeme() == "true" }
    {
    }

LeafBool::LeafBool(bool value) :
    LeafObject { nullptr },
    m_value { value }
    {
    }

auto LeafBool::type() -> ObjectType {
    return k_bool;
}

auto LeafBool::is_truthy() -> bool {
    return m_value;
}

auto LeafBool::value() -> bool {
    return m_value;
}
