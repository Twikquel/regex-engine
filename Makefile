all: build

build: main.cpp postfixconverter.cpp postfixconverter.h
	g++ -o main -Wall -Werror postfixconverter.cpp regex_to_nfa.cpp main.cpp

clean:
	rm -f main