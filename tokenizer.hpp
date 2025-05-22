#ifndef leaf_tokenizer_hpp
#define leaf_tokenizer_hpp

#include "token.hpp"

#include <string_view>
#include <vector>

namespace tokenizer {
    auto get_tokens(std::string_view source) -> std::vector<Token*>&;
    auto reset() -> void;
};

#endif
