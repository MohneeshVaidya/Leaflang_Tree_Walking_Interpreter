#ifndef ERROR_HPP
#define ERROR_HPP

#include "Token.hpp"

#include <string>
#include <string_view>
#include <vector>

class Error {
private:
    static Error* instance;

    // Private methods
    Error() = default;

public:
    std::vector<std::string> errors { };
    bool has_errors { false };

    // Public methods
    ~Error();

    static Error* get_instance();

    void add_error(const Token& token, std::string_view message);
};

#endif
