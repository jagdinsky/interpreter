all:
	g++ -std=c++11 core.cpp support.cpp -o program -Wall -fsanitize=address,leak,undefined

