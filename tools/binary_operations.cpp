#include "binary_operations.hpp"
#include "../leaf_error.hpp"

#include <cmath>
#include <utility>
#include <cstdint>

using namespace std::string_literals;

using enum TokenType;

// Initialize static variable m_instance
const BinaryOperations* BinaryOperations::m_instance { nullptr };

// Static methods
auto BinaryOperations::instance() -> const BinaryOperations* {
    if (m_instance == nullptr) {
        m_instance = new BinaryOperations { };
    }
    return m_instance;
}

auto BinaryOperations::delete_instance() -> void {
    delete m_instance;
    m_instance = nullptr;
}

// Constructor
BinaryOperations::BinaryOperations() {
    m_operations_table = {
        std::make_pair(k_or, [](OperandType left, OperandType right) -> ReturnType {
            return LeafBool::create_object(left->is_truthy() || right->is_truthy());
        }),

        std::make_pair(k_and, [](OperandType left, OperandType right) -> ReturnType {
            return LeafBool::create_object(left->is_truthy() && right->is_truthy());
        }),

        std::make_pair(k_equal_equal, [](OperandType left, OperandType right) -> ReturnType {
            bool result { false };

            if (left->type() == right->type()) {
                if (left->type() == ObjectType::k_null) {
                    result = true;
                }
                if (left->type() == ObjectType::k_number) {
                    auto casted_left  { dynamic_cast<const LeafNumber*>(left) };
                    auto casted_right { dynamic_cast<const LeafNumber*>(right) };

                    result = casted_left->value() == casted_right->value();
                }
                if (left->type() == ObjectType::k_string) {
                    auto casted_left  { dynamic_cast<const LeafString*>(left) };
                    auto casted_right { dynamic_cast<const LeafString*>(right) };

                    result = casted_left->value() == casted_right->value();
                }
                if (left->type() == ObjectType::k_bool) {
                    auto casted_left  { dynamic_cast<const LeafBool*>(left) };
                    auto casted_right { dynamic_cast<const LeafBool*>(right) };

                    result = casted_left->value() == casted_right->value();
                }
            }

            return LeafBool::create_object(result);
        }),

        std::make_pair(k_bang_equal, [](OperandType left, OperandType right) -> ReturnType {
            bool result { true };

            if (left->type() == right->type()) {
                if (left->type() == ObjectType::k_null) {
                    result = false;
                }
                if (left->type() == ObjectType::k_number) {
                    auto casted_left  { dynamic_cast<const LeafNumber*>(left) };
                    auto casted_right { dynamic_cast<const LeafNumber*>(right) };

                    result = casted_left->value() != casted_right->value();
                }
                if (left->type() == ObjectType::k_string) {
                    auto casted_left  { dynamic_cast<const LeafString*>(left) };
                    auto casted_right { dynamic_cast<const LeafString*>(right) };

                    result = casted_left->value() != casted_right->value();
                }
                if (left->type() == ObjectType::k_bool) {
                    auto casted_left  { dynamic_cast<const LeafBool*>(left) };
                    auto casted_right { dynamic_cast<const LeafBool*>(right) };

                    result = casted_left->value() != casted_right->value();
                }
            }

            return LeafBool::create_object(result);
        }),

        std::make_pair(k_lesser, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafBool::create_object(casted_left->value() < casted_right->value());
        }),

        std::make_pair(k_lesser_equal, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafBool::create_object(casted_left->value() <= casted_right->value());
        }),

        std::make_pair(k_greater, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafBool::create_object(casted_left->value() > casted_right->value());
        }),

        std::make_pair(k_greater_equal, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafBool::create_object(casted_left->value() >= casted_right->value());
        }),

        std::make_pair(k_plus, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number, ObjectType::k_string });

            if (left->type() == ObjectType::k_number) {
                auto casted_left { dynamic_cast<const LeafNumber*>(left) };
                auto casted_right { dynamic_cast<const LeafNumber*>(right) };

                return LeafNumber::create_object(casted_left->value() + casted_right->value());
            }

            auto casted_left { dynamic_cast<const LeafString*>(left) };
            auto casted_right { dynamic_cast<const LeafString*>(right) };

            return LeafString::create_object(casted_left->value() + casted_right->value());
        }),

        std::make_pair(k_minus, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafNumber::create_object(casted_left->value() - casted_right->value());
        }),

        std::make_pair(k_star, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafNumber::create_object(casted_left->value() * casted_right->value());
        }),

        std::make_pair(k_slash, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafNumber::create_object(casted_left->value() / casted_right->value());
        }),

        std::make_pair(k_percent, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafNumber::create_object(static_cast<double>(static_cast<int64_t>(casted_left->value()) % static_cast<int64_t>(casted_right->value())));
        }),

        std::make_pair(k_star_star, [this](OperandType left, OperandType right) -> ReturnType {
            expect_same_type(left, right);
            expect_type(left->type(), { ObjectType::k_number });

            auto casted_left { dynamic_cast<const LeafNumber*>(left) };
            auto casted_right { dynamic_cast<const LeafNumber*>(right) };

            return LeafNumber::create_object(std::pow(casted_left->value(), casted_right->value()));
        }),
    };
}

// Private methods
auto BinaryOperations::expect_same_type(OperandType left, OperandType right) const -> void {
    if (left->type() != right->type()) {
        throw ErrorCode::k_type_mismatch;
    }
}

auto BinaryOperations::expect_type(ObjectType type, std::initializer_list<ObjectType> expected) const -> void {
    for (ObjectType expected_type : expected) {
        if (type == expected_type) {
            return;
        }
    }
    throw ErrorCode::k_invalid_type;
}

// Public methods
auto BinaryOperations::execute(const Token* oper, OperandType left, OperandType right) const -> ReturnType {
    if (left == nullptr || right == nullptr) {
        LeafError::instance()->runtime_error(oper->line(), oper->lexeme(), "Operands of binary operator must not be absent"s);
    }

    try {
        return m_operations_table.at(oper->type())(left, right);
    } catch (const ErrorCode error_code) {
        if (error_code == ErrorCode::k_type_mismatch) {
            LeafError::instance()->runtime_error(oper->line(), oper->lexeme(), "Operands of binary operator must by of same type.");
        } else if (error_code == ErrorCode::k_invalid_type) {
            LeafError::instance()->runtime_error(oper->line(), oper->lexeme(), "Operands of binary operator have invalid types.");
        }
    }
    return nullptr;
}
