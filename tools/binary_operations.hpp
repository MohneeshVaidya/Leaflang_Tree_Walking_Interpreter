#ifndef BINARY_OPERATIONS_HPP
#define BINARY_OPERATIONS_HPP

#include "../token_type.hpp"
#include "../leaf_object.hpp"
#include "../token.hpp"

#include <functional>
#include <initializer_list>
#include <unordered_map>

using ReturnType = LeafObject*;
using OperandType = LeafObject*;
using OperationsTableType = std::unordered_map<TokenType, std::function<ReturnType(OperandType, OperandType)>>;

class BinaryOperations {
private:
    static BinaryOperations* m_instance;

private:
    OperationsTableType m_operations_table { };

    // Constructor
    BinaryOperations();

    // Private methods
    auto expect_same_type(OperandType left, OperandType right) -> void;
    auto expect_type(ObjectType type, std::initializer_list<ObjectType> expected) -> void;

public:
    static auto instance() -> BinaryOperations*;
    static auto delete_instance() -> void;

    auto execute(Token* oper, OperandType left, OperandType right) -> ReturnType;
};

enum class ErrorCode {
    k_type_mismatch,
    k_invalid_type,
};

#endif
