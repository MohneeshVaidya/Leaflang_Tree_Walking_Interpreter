leaf:	main.cpp TokenType.cpp Lexer.cpp Token.cpp tools/Tools.cpp Error.cpp
	c++ -std=c++23 -g -Wall -Wextra -Weffc++ -Werror -Wconversion -Wsign-conversion main.cpp TokenType.cpp Lexer.cpp Token.cpp tools/Tools.cpp Error.cpp -o leaf

clean:
	rm -rf ./leaf
