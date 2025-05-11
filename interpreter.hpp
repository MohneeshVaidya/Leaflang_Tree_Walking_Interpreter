#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "environment.hpp"
#include "expr.hpp"
#include "leaf_object.hpp"
#include "leaf_struct.hpp"
#include "stmt.hpp"

#include <vector>


// BlockCtx
enum class BlockCtx {
    k_none,
    k_loop,
};

// FunctionCtx
enum class CalledCtx {
    k_none,
    k_function,
    k_constructor,
    k_method,
};


// Interpreter
class Interpreter : public ExprVisitor, public StmtVisitor {
    friend class LeafFunction;

private:
    Environment* m_environment { };
    BlockCtx m_block_ctx { BlockCtx::k_none };
    CalledCtx m_called_ctx { CalledCtx::k_none };
    LeafStructInstance* m_current_instance { nullptr };
    bool m_is_method { false };

    auto execute_stmt(Stmt* stmt) -> void;
    auto evaluate(Expr* expr) -> LeafObject*;

    auto operator = (Interpreter& other) -> Interpreter& = default;

public:
    Interpreter(Environment* environment);
    Interpreter(Interpreter& source) = default;
    ~Interpreter();

    virtual auto execute(std::vector<Stmt*>& statements) -> void override;
    virtual auto execute(std::vector<Expr*>& expressions) -> LeafObject* override;

    virtual auto visit_printstmt(PrintStmt* stmt) -> void override;
    virtual auto visit_printlnstmt(PrintlnStmt* stmt) -> void override;
    virtual auto visit_expressionstmt(ExpressionStmt* stmt) -> void override;
    virtual auto visit_varstmt(VarStmt* stmt) -> void override;
    virtual auto visit_conststmt(ConstStmt* stmt) -> void override;
    virtual auto visit_blockstmt(BlockStmt* stmt, Environment* environment) -> void override;
    virtual auto visit_ifstmt(IfStmt* stmt) -> void override;
    virtual auto visit_forstmt(ForStmt* stmt) -> void override;
    virtual auto visit_breakstmt(BreakStmt* stmt) -> void override;
    virtual auto visit_continuestmt(ContinueStmt* stmt) -> void override;
    virtual auto visit_returnstmt(ReturnStmt* stmt) -> void override;

    virtual auto visit_nullexpr(NullExpr* expr) -> LeafObject* override;
    virtual auto visit_assignexpr(AssignExpr* expr) -> LeafObject* override;
    virtual auto visit_ternaryexpr(TernaryExpr* expr) -> LeafObject* override;
    virtual auto visit_binaryexpr(BinaryExpr* expr) -> LeafObject* override;
    virtual auto visit_unaryexpr(UnaryExpr* expr) -> LeafObject* override;
    virtual auto visit_exponentexpr(ExponentExpr* expr) -> LeafObject* override;
    virtual auto visit_groupingexpr(GroupingExpr* expr) -> LeafObject* override;
    virtual auto visit_primaryexpr(PrimaryExpr* expr) -> LeafObject* override;
    virtual auto visit_functionexpr(FunctionExpr* expr) -> LeafObject* override;
    virtual auto visit_callexpr(CallExpr* expr) -> LeafObject* override;
    virtual auto visit_structexpr(StructExpr* expr) -> LeafObject* override;
    virtual auto visit_getexpr(GetExpr* expr) -> LeafObject* override;
    virtual auto visit_setexpr(SetExpr* expr) -> LeafObject* override;
};

#endif
