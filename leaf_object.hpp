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
};


// LeafObject classes
class LeafObject {
protected:
    const Token* m_token;

    LeafObject(const Token* token);
    LeafObject(const LeafObject& source) = default;

public:
    static auto delete_object(const LeafObject* object) -> void;

    virtual ~LeafObject();
    virtual auto type() const -> ObjectType = 0;
    virtual auto is_truthy() const -> bool = 0;

    auto operator = (const LeafObject& other) -> LeafObject& = default;
};

auto operator << (std::ostream& stream, const LeafObject* object) -> std::ostream&;


// LeafNull
class LeafNull : public LeafObject {
private:
    LeafNull(const Token* token);
    LeafNull();

public:
    static auto create_object(const Token* token) -> LeafNull*;
    static auto create_object() -> LeafNull*;

    virtual auto type() const -> ObjectType override;
    virtual auto is_truthy() const -> bool override;
};


// LeafNumber
class LeafNumber : public LeafObject {
private:
    const double m_value;

    LeafNumber(const Token* token);
    LeafNumber(const double value);

public:
    static auto create_object(const Token* token) -> LeafNumber*;
    static auto create_object(const double value) -> LeafNumber*;

    virtual auto type() const -> ObjectType override;
    virtual auto is_truthy() const -> bool override;

    auto value() const -> double;
};


// LeafString
class LeafString : public LeafObject {
private:
    const std::string m_value;

    LeafString(const Token* token);
    LeafString(const std::string& value);

public:
    static auto create_object(const Token* token) -> LeafString*;
    static auto create_object(const std::string& value) -> LeafString*;

    virtual auto type() const -> ObjectType override;
    virtual auto is_truthy() const-> bool override;

    auto value() const -> const std::string&;
};


// LeafBool
class LeafBool : public LeafObject {
private:
    const bool m_value;

    LeafBool(const Token* token);
    LeafBool(const bool value);

public:
    static auto create_object(const Token* token) -> LeafBool*;
    static auto create_object(const bool value) -> LeafBool*;

    virtual auto type() const -> ObjectType override;
    virtual auto is_truthy() const-> bool override;

    auto value() const -> bool;
};


// LeafIdentifier
// class LeafIdentifier : public LeafObject {
// private:
//     const std::string& m_name;
//     const LeafObject* m_value;
//
//     LeafIdentifier(const std::string& name, const LeafObject* value);
//     LeafIdentifier(const LeafIdentifier& source) = default;
//
// public:
//     static auto create_object(const std::string& name, const LeafObject* value) -> LeafIdentifier*;
//
//     virtual auto type() const -> ObjectType override;
//     virtual auto is_truthy() const -> bool override;
//
//     auto name() const -> const std::string&;
//     auto value() const -> const LeafObject*;
//
//     auto operator = ([[maybe_unused]] const LeafIdentifier& other) -> LeafIdentifier& {
//         return *this;
//     };
// };

#endif
