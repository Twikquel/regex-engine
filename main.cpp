#include <iostream>
#include <string>
#include "postfixconverter.h"

int main(int argc, char *argv[]) {
    std::string temp = addExplicitConcat(argv[1]);
    std::cout << temp << std::endl;

    return 0;
}