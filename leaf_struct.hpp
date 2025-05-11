#ifndef LEAF_STRUCT_HPP
#define LEAF_STRUCT_HPP

#include "expr.hpp"
#include "leaf_object.hpp"
#include "token.hpp"

#include <unordered_map>

// StructExpr
class StructExpr : public Expr {
public:
    Token* identifier;
    std::vector<Token*> fields;
    std::vector<FunctionExpr*> methods;

private:
    StructExpr(Token* identifier, std::vector<Token*>& fields, std::vector<FunctionExpr*>& methods);
    StructExpr(StructExpr& source) = default;
    auto operator = (StructExpr& other) -> StructExpr& = default;

public:
    ~StructExpr();
    static auto create_object(Token* identifier, std::vector<Token*>& fields, std::vector<FunctionExpr*>& methods) -> StructExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// GetExpr
class GetExpr : public Expr {
public:
    Token* left;
    Expr* left_expr;
    Token* right;
    Expr* right_expr;

private:
    GetExpr(Token* left, Expr* left_expr, Token* right, Expr* right_expr);
    GetExpr(GetExpr&) = default;
    auto operator = (GetExpr&) -> GetExpr& = default;

public:
    ~GetExpr();
    static auto create_object(Token* left, Expr* left_expr, Token* right, Expr* right_expr) -> GetExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// SetExpr
class SetExpr : public Expr {
public:
    GetExpr* left;
    Token* oper;
    Expr* right;

private:
    SetExpr(GetExpr* left, Token* oper, Expr* right);
    SetExpr(SetExpr&) = default;
    auto operator = (SetExpr&) -> SetExpr& = default;

public:
    ~SetExpr();
    static auto create_object(GetExpr* left, Token* oper, Expr* right) -> SetExpr*;

    virtual auto accept(ExprVisitor* visitor) -> LeafObject* override;
    virtual auto type() -> ExprType override;
};


// LeafStruct
class LeafStruct : public LeafObject {
public:
    Token* identifier;

private:
    std::vector<Token*> m_fields;

    LeafStruct(Token* identifier, std::vector<Token*>& fields);
    LeafStruct(LeafStruct& source) = default;
    auto operator = (LeafStruct& other) -> LeafStruct& = default;

public:
    static auto create_object(Token* identifier, std::vector<Token*>& fields) -> LeafStruct*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto value() -> std::vector<Token*>&;
};


// LeafStructInstance
class LeafStructInstance : public LeafObject {
private:
    Token* m_struct_name;
    std::unordered_map<std::string, LeafObject*> m_fields;

private:
    LeafStructInstance(Token* struct_name, std::unordered_map<std::string, LeafObject*>& fields);
    LeafStructInstance(LeafStructInstance&) = default;
    auto operator = (LeafStructInstance&) -> LeafStructInstance& = default;

public:
    static auto create_object(Token* struct_identifier, std::unordered_map<std::string, LeafObject*>& fields) -> LeafStructInstance*;

    virtual auto type() -> ObjectType override;
    virtual auto is_truthy() -> bool override;

    auto struct_name() -> Token*;
    auto value() -> std::unordered_map<std::string, LeafObject*>&;
};

#endif
