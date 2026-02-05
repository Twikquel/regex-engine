all: build

build: main.cpp postfixconverter.cpp postfixconverter.h
	g++ -o main -Wall -Werror main.cpp postfixconverter.cpp

clean:
	rm -f main