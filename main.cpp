#include <iostream>
#include <queue>
#include "postfixconverter.h"

int main(int argc, char *argv[]) {
    std::queue<char> temp = convertToPostfix(argv[1]);

    while(!temp.empty()) {
        std::cout << temp.front();
        temp.pop();
    }
    std::cout << std::endl;

    return 0;
}