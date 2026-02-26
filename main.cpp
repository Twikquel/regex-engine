#include <iostream>
#include "postfixconverter.h"
#include "regex_to_nfa.h"
#include "nfa_to_dfa.h"

void printNFA(NFA nfa) {
    std::cout << "Initial State: " << nfa.initialState << std::endl;
    std::cout << "Accepting State: " << nfa.acceptingState << std::endl;

    std::cout << "Alphabet: " << "{ ";
    for (const auto symbol : nfa.alphabet) {
        std::cout << symbol << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Transition Function:" << std::endl;
    for (const auto& [key, value] : nfa.transitionFunction) {
        std::cout << "\t(" << key.state << ", " << key.input << ") --> { "; 
        for(int32_t i: value) {
            std::cout << i << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;
}

void printDFA(DFA dfa) {
    std::cout << "Initial State: " << dfa.initialState << std::endl;
    std::cout << "Accepting States: { ";
    for(int32_t state : dfa.acceptingStates) {
        std::cout << state << " "; 
    }
    std:: cout << "}" << std::endl;

    std::cout << "Alphabet: " << "{ ";
    for (const auto symbol : dfa.alphabet) {
        std::cout << symbol << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Transition Function:" << std::endl;
    for (const auto& [key, value] : dfa.transitionFunction) {
        std::cout << "\t(" << key.state << ", " << key.input << ") --> " << value << std::endl; 
    }
    std::cout << std::endl;
}

int main(int32_t argc, char *argv[]) {
    std::cout << "Regex: " << argv[1] << std::endl;

    std::queue<char> postfixRegex = convertToPostfix(argv[1]);

    NFA nfa = convertToNFA(postfixRegex);
    DFA dfa = convertToDFA(nfa);

    printNFA(nfa);
    printDFA(dfa);

    std::cout << dfa.run("a") << std::endl;
    std::cout << dfa.run("aaaa") << std::endl;
    std::cout << dfa.run("aaaaab") << std::endl;
    std::cout << dfa.run("c") << std::endl;

    return 0;
}