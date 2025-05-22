leaf:	main.cpp token_type.cpp token.cpp tokenizer.cpp
	g++ -std=c++23 -g -Wall -Wextra -Weffc++ -Wconversion -Wsign-conversion -Werror main.cpp token_type.cpp token.cpp tokenizer.cpp -o leaf

bear:
	bear -- make

clean:
	rm -rf leaf

run:
	./leaf ./programs/test.txt
