all: build

build: main.cpp postfixconverter.cpp
	g++ -o main main.cpp postfixconverter.cpp

clean:
	rm -f main