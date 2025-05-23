#ifndef leaf_parser_hpp
#define leaf_parser_hpp

#include "expr.hpp"
#include "token.hpp"

#include <vector>

namespace parser {
auto get_stmts(std::vector<Token*>& tokens) -> std::vector<Expr*>&;
auto reset() -> void;
};

#endif
