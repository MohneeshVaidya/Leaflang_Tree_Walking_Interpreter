leaf:   main.cpp token_type.cpp token.cpp tokenizer.cpp tools/leaf_tools.cpp leaf_error.cpp expr.cpp parser.cpp mathexpr_printer.cpp
	g++ -std=c++23 -g -Wall -Wextra -Weffc++ -Werror -Wconversion -Wsign-conversion main.cpp token_type.cpp token.cpp tokenizer.cpp tools/leaf_tools.cpp leaf_error.cpp expr.cpp parser.cpp mathexpr_printer.cpp -o leaf

clean:
	rm -rf ./leaf

bear:
	bear -- make
