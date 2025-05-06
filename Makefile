leaf:   main.cpp token_type.cpp token.cpp tokenizer.cpp tools/leaf_tools.cpp leaf_error.cpp leaf_object.cpp expr.cpp stmt.cpp parser.cpp expr_printer.cpp interpreter.cpp tools/binary_operations.cpp
	g++ -std=c++23 -g -Wall -Wextra -Weffc++ -Werror -Wconversion -Wsign-conversion main.cpp token_type.cpp token.cpp tokenizer.cpp tools/leaf_tools.cpp leaf_error.cpp leaf_object.cpp expr.cpp stmt.cpp parser.cpp expr_printer.cpp interpreter.cpp tools/binary_operations.cpp -o leaf

clean:
	rm -rf ./leaf

bear:
	bear -- make
