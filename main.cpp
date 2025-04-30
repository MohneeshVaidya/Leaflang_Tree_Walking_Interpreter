#include "Lexer.hpp"
#include "Token.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

void run_repl();
void run_file(std::string_view file_path);
void run(std::string_view source_code);

int main(int argc, char* argv[]) {
    if (argc >= 3 || (argc == 2 && std::strcmp(argv[1], "--help") == 0)) {
        std::cout << "Usage 1: ./leaf <fileName> (To interpret source file).\n";
        std::cout << "Usage 2: ./leaf (To invoke REPL)\n";
        std::exit(1);
    }

    if (argc == 1) {
        run_repl();
        std::exit(0);
    }

    run_file(argv[1]);

    return 0;
}

void run_repl() {
    std::string expression { };

    while (true) {
        std::cout << "Leaf > ";
        std::getline(std::cin, expression, '\n');
        if (expression == "exit") {
            std::cout << "Exiting Repl.\n";
            std::exit(0);
        }
        run(expression);
    }
}

void run_file(std::string_view file_path) {
    std::ifstream file { file_path.data() };

    if (!file.is_open()) {
        std::cerr << "Error: file '" << file_path << "' couldn't be opened, check if this file exists or not.\n";
        std::exit(1);
    }

    std::string source_code { };
    std::string line { };

    while (!file.eof()) {
        std::getline(file, line, '\n');

        source_code.append(line);
        source_code.push_back('\n');
    }

    run(source_code);

    file.close();
}

void run(std::string_view source_code) {
    Lexer lexer { source_code };
    std::vector<Token> tokens { lexer.get_tokens() };
    for (Token token : tokens) {
        std::cout << token << "\n";
    }
}
