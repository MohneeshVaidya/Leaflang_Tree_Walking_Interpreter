leaf:	main.cpp token_type.cpp token.cpp tokenizer.cpp error.cpp expr.cpp object.cpp parser.cpp
	g++ -std=c++23 -g -Wall -Wextra -Weffc++ -Wconversion -Wsign-conversion -Werror main.cpp token_type.cpp token.cpp tokenizer.cpp error.cpp expr.cpp object.cpp parser.cpp -o leaf

bear:
	bear -- make

clean:
	rm -rf leaf

run:
	./leaf ./programs/test.txt
