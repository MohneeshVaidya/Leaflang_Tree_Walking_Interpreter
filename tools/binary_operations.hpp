#ifndef BINARY_OPERATIONS_HPP
#define BINARY_OPERATIONS_HPP

#include "../token_type.hpp"
#include "../leaf_object.hpp"
#include "../token.hpp"

#include <functional>
#include <initializer_list>
#include <unordered_map>

using ReturnType = LeafObject*;
using OperandType = const LeafObject*;
using OperationsTableType = std::unordered_map<TokenType, std::function<ReturnType(OperandType, OperandType)>>;

class BinaryOperations {
private:
    static const BinaryOperations* m_instance;

private:
    OperationsTableType m_operations_table { };

    // Constructor
    BinaryOperations();

    // Private methods
    auto expect_same_type(OperandType left, OperandType right) const -> void;
    auto expect_type(ObjectType type, std::initializer_list<ObjectType> expected) const -> void;

public:
    static auto instance() -> const BinaryOperations*;
    static auto delete_instance() -> void;

    auto execute(const Token* oper, OperandType left, OperandType right) const -> ReturnType;
};

enum class ErrorCode {
    k_type_mismatch,
    k_invalid_type,
};

#endif
