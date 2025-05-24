#include "error.hpp"
#include "expr.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

auto run_repl() -> void;
auto run_file(std::string_view path) -> void;
auto run(std::string_view source) -> void;
auto cleanup() -> void;

auto main(int argc, char* argv[]) -> int {
    if (argc == 2 && std::strcmp(argv[1], "--help") == 0) {
        std::cout << "usage 1: leaf (to start repl).\n";
        std::cout << "usage 2: leaf <filePath> (to execute a file).\n";
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
        std::cout << "leaf > ";
        std::getline(std::cin, source, '\n');
        if (source.compare("exit") == 0) {
            std::cout << "exiting repl.\n";
            std::exit(0);
        }
        run(source);
    }
}

auto run_file(std::string_view path) -> void {
    std::fstream file { path.data(), std::ios::in };
    if (file.is_open() == false) {
        std::cerr << std::format("error: not able to open file '{}'\n", path);
        std::exit(1);
    }
    std::string source { };
    std::string line { };
    while (file.eof() == false) {
        std::getline(file, line, '\n');
        line.push_back('\n');
        source.append(line);
    }
    run(source);
    file.close();
}

auto run(std::string_view source) -> void {
    error::init();

    std::vector<Token*>& tokens { tokenizer::get_tokens(source) };

    if (error::has_errors()) {
        error::print();
    } else {
        std::vector<Expr*>& stmts { parser::get_stmts(tokens) };

        if (error::has_errors()) {
            error::print();
        } else {
            Printer printer { };
            printer.execute(stmts);
        }
    }

    error::reset();
    tokenizer::reset();
    parser::reset();
}

auto cleanup() -> void {

}
