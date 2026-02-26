all: build

build: main.cpp postfixconverter.cpp postfixconverter.h
	g++ -o main -Wall -Werror postfixconverter.cpp regex_to_nfa.cpp nfa_to_dfa.cpp main.cpp

test: test.cpp postfixconverter.cpp postfixconverter.h
	g++ -o test -Wall -Werror postfixconverter.cpp regex_to_nfa.cpp nfa_to_dfa.cpp test.cpp
	./test

clean:
	rm -f main
	rm -f test