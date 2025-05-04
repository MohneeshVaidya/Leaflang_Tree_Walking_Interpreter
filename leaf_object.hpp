#ifndef LEAF_OBJECT_HPP
#define LEAF_OBJECT_HPP

#include <string>

enum class ObjectType {
    k_null,
    k_number,
    k_string,
    k_bool,
};

struct LeafObject {
    const ObjectType type;

protected:
    LeafObject(ObjectType type) : type { type } { }
};

struct LeafNull : public LeafObject {
    LeafNull() : LeafObject { ObjectType::k_null } { }
};

struct LeafNumber : public LeafObject {
    const double value;

    LeafNumber(const double value) : LeafObject { ObjectType::k_number }, value { value } { }
};

struct LeafString : public LeafObject {
    const std::string value;

    LeafString(const std::string& value) : LeafObject { ObjectType::k_string }, value { value } { }
};

struct LeafBool : public LeafObject {
    const bool value;

    LeafBool(const bool value) : LeafObject { ObjectType::k_bool }, value { value } { }
};

#endif
