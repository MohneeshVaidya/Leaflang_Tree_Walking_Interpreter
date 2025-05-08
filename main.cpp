#include "interpreter.hpp"
#include "leaf_error.hpp"
#include "parser.hpp"
#include "stmt.hpp"
#include "tokenizer.hpp"
#include "token.hpp"
#include "tools/binary_operations.hpp"

#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

auto run_repl() -> void;
auto run_file(const std::string& filepath) -> void;
auto run(const std::string& source) -> void;

auto print_errors() -> void;
auto print_tokens(const std::vector<const Token*>& tokens) -> void;

auto cleanup() -> void;

int main(int argc, char* argv[]) {
    if (argc > 2 || (argc > 1 && std::strcmp(argv[1], "--help") == 0)) {
        std::cout << "Usage 1: ./leaf <fileName> (To execute a file.)\n";
        std::cout << "Usage 2: ./leaf (To run Repl (Run eval print loop)).\n";
        std::exit(0);
    }

    std::atexit(cleanup);

    if (argc == 1) {
        run_repl();
    }

    run_file(argv[1]);
    return 0;
}

auto run_repl() -> void {
    std::string source { };

    while (true) {
        std::cout << "Leaf > ";
        std::getline(std::cin, source, '\n');

        if (source == "exit" || source == "exit;") {
            std::cout << "Exiting Repl.\n";
            std::exit(0);
        }

        run(source);
    }
}

auto run_file(const std::string& filepath) -> void {
    std::ifstream file { filepath };

    if (file.is_open() == false) {
        std::cerr << std::format("Not able to open the file {}.\n", filepath);
        std::abort();
    }

    std::string source { };
    std::string line   { };

    while (file.eof() == false) {
        std::getline(file, line, '\n');
        source.append(line);
        source.push_back('\n');
    }

    run(source);

    file.close();
}

auto run(const std::string& source) -> void {
    Tokenizer tokenizer { source };
    std::vector<const Token*> tokens { tokenizer.tokenize().tokens() };

    print_errors();

    Parser parser { tokens };
    const std::vector<const Stmt*> statements { parser.parse().statements() };

    print_errors();

    std::cout << std::boolalpha;
    std::cout << std::setprecision(20);

    try {
        Interpreter interpreter { Environment::create_object(nullptr) };
        interpreter.execute(statements);
    } catch (std::string& runtime_error) {
        std::cerr << runtime_error << "\n";
        std::exit(1);
    }
}

auto print_errors() -> void {
    if (LeafError::instance()->has_errors()) {
        for (const std::string& message : LeafError::instance()->messages()) {
            std::cerr << message << "\n";
        }
        std::exit(1);
    }
}

auto print_tokens(const std::vector<const Token*>& tokens) -> void {
    for (const Token* token : tokens) {
        std::cout << *token << "\n";
    }
}

auto cleanup() -> void {
    std::cout << std::noboolalpha;
    std::cout << std::setprecision(6);

    LeafError::delete_instance();
    BinaryOperations::delete_instance();
}
