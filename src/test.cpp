#include <iostream>
#include <cassert>
#include "test_postfix.h"
#include "test_nfa.h"
#include "test_dfa.h"


int main() {
    test_postfix();
    std::cout << "Postfix tests passed" << std::endl;
    test_nfa();
    std::cout << "NFA tests passed" << std::endl;
    test_dfa();
    std::cout << "DFA tests passed" << std::endl;

    std::cout << "All tests passed" << std::endl;
} 
