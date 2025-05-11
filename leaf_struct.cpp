#include "leaf_struct.hpp"
#include "expr.hpp"
#include "leaf_function.hpp"
#include "leaf_object.hpp"
#include "token.hpp"

// StructExpr
StructExpr::StructExpr(Token* identifier, std::vector<Token*>& fields, std::vector<FunctionExpr*>& methods) :
    identifier { identifier },
    fields { fields },
    methods { methods }
    {
    }

StructExpr::~StructExpr() {
    for (auto method : methods) {
        Expr::delete_object(method);
    }
}

auto StructExpr::create_object(Token* identifier, std::vector<Token*>& fields, std::vector<FunctionExpr*>& methods) -> StructExpr* {
    return new StructExpr { identifier, fields, methods };
}

auto StructExpr::accept(ExprVisitor* visitor) ->LeafObject* {
    return visitor->visit_structexpr(this);
}

auto StructExpr::type() -> ExprType {
    return ExprType::k_struct;
}


// GetExpr
GetExpr::GetExpr(Token* left, Expr* left_expr, Token* right, Expr* right_expr) :
    left { left },
    left_expr { left_expr },
    right { right },
    right_expr { right_expr }
    {
    }

GetExpr::~GetExpr() {
    Expr::delete_object(left_expr);
    Expr::delete_object(right_expr);
}

auto GetExpr::create_object(Token* left, Expr* left_expr, Token* right, Expr* right_expr) -> GetExpr* {
    return new GetExpr { left, left_expr, right, right_expr };
}

auto GetExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_getexpr(this);
}

auto GetExpr::type() -> ExprType {
    return ExprType::k_get;
}


// SetExpr
SetExpr::SetExpr(GetExpr* left, Token* oper, Expr* right) :
    left { left },
    oper { oper },
    right { right }
    {
    }

SetExpr::~SetExpr() {
    Expr::delete_object(left);
    Expr::delete_object(right);
}

auto SetExpr::create_object(GetExpr* left, Token* oper, Expr* right) -> SetExpr* {
    return new SetExpr { left, oper, right };
}

auto SetExpr::accept(ExprVisitor* visitor) -> LeafObject* {
    return visitor->visit_setexpr(this);
}

auto SetExpr::type() -> ExprType {
    return ExprType::k_set;
}


// LeafStruct
LeafStruct::LeafStruct(Token* identifier, std::vector<Token*>& fields) :
    LeafObject { nullptr },
    identifier { identifier },
    m_fields { fields }
    {
    }

auto LeafStruct::create_object(Token* identifier, std::vector<Token*>& fields) -> LeafStruct* {
    return new LeafStruct { identifier, fields };
}

auto LeafStruct::type() -> ObjectType {
    return ObjectType::k_struct;
}

auto LeafStruct::is_truthy() -> bool {
    return true;
}

auto LeafStruct::value() -> std::vector<Token*>& {
    return m_fields;
}


// LeafStructInstance
LeafStructInstance::LeafStructInstance(Token* struct_name, std::unordered_map<std::string, LeafObject*>& fields) :
    LeafObject { nullptr },
    m_struct_name { struct_name },
    m_fields { fields }
    {
    }

auto LeafStructInstance::create_object(Token* struct_name, std::unordered_map<std::string, LeafObject*>& fields) -> LeafStructInstance* {
    return new LeafStructInstance { struct_name, fields };
}

auto LeafStructInstance::type() -> ObjectType {
    return ObjectType::k_struct_instance;
}

auto LeafStructInstance::is_truthy() -> bool {
    return true;
}

auto LeafStructInstance::struct_name() -> Token* {
    return m_struct_name;
}

auto LeafStructInstance::value() -> std::unordered_map<std::string, LeafObject*>& {
    return m_fields;
}
