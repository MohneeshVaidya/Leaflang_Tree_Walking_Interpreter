#ifndef LEAF_OBJECT_HPP
#define LEAF_OBJECT_HPP

#include "token.hpp"

#include <ostream>
#include <string>

enum class ObjectType {
    k_null,
    k_number,
    k_string,
    k_bool,
    k_identifier,
    k_function,
    k_struct,
    k_struct_instance,
};


// LeafObject classes
class LeafObject {
protected:
    Token* m_token;

    LeafObject(Token* token);
    LeafObject(LeafObject& source) = default;
    auto operator = (LeafObject& other) -> LeafObject& = default;

public:
    static auto delete_object(LeafObject* object) -> void;

    virtual ~LeafObject();
    virtual auto type() -> ObjectType = 0;
    virtual auto is_truthy() -> bool = 0;
};

auto operator << (std::ostream& stream, LeafObject* object) -> std::ostream&;


// LeafNull
class LeafNull : public LeafObject {
private:
    LeafNull(Token* token);
    LeafNull();

public:
    static auto create_object(Token* token) -> LeafNull*;
    static auto create_object() -> LeafNull*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;
};


// LeafNumber
class LeafNumber : public LeafObject {
private:
    double m_value;

    LeafNumber(Token* token);
    LeafNumber(double value);

public:
    static auto create_object(Token* token) -> LeafNumber*;
    static auto create_object(double value) -> LeafNumber*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto value() -> double;
};


// LeafString
class LeafString : public LeafObject {
private:
    std::string m_value;

    LeafString(Token* token);
    LeafString(const std::string& value);

public:
    static auto create_object(Token* token) -> LeafString*;
    static auto create_object(const std::string& value) -> LeafString*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto value() -> std::string&;
};


// LeafBool
class LeafBool : public LeafObject {
private:
    bool m_value;

    LeafBool(Token* token);
    LeafBool(bool value);

public:
    static auto create_object(Token* token) -> LeafBool*;
    static auto create_object(bool value) -> LeafBool*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto value() -> bool;
};

#endif
