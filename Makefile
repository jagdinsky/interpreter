all:
	g++ -std=c++11 usr/interpreter.cpp src/intlib.cpp \-I headers/ -o bin/interpreter -Wall -fsanitize=address,leak,undefined
