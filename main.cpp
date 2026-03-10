#include <iostream>
#include <chrono>
#include <thread>
#include "postfixconverter.h"
#include "regex_to_nfa.h"
#include "nfa_to_dfa.h"
#include "minimize_dfa.h"

void printNFA(NFA nfa) {
    std::cout << "States: " << "{ ";
    for (const auto state : nfa.states) {
        std::cout << state << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Initial State: " << nfa.initial_state << std::endl;
    std::cout << "Accepting State: " << nfa.accepting_state << std::endl;

    std::cout << "Alphabet: " << "{ ";
    for (const auto symbol : nfa.alphabet) {
        std::cout << symbol << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Transition Function:" << std::endl;
    for (const auto& [key, value] : nfa.transition_function) {
        std::cout << "\t(" << key.state << ", " << key.input << ") --> { "; 
        for(int32_t i: value) {
            std::cout << i << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;
}

void printDFA(DFA dfa) {
    std::cout << "States: " << "{ ";
    for (const auto state : dfa.states) {
        std::cout << state << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Initial State: " << dfa.initial_state << std::endl;
    std::cout << "Accepting States: { ";
    for(int32_t state : dfa.accepting_states) {
        std::cout << state << " "; 
    }
    std:: cout << "}" << std::endl;

    std::cout << "Alphabet: " << "{ ";
    for (const auto symbol : dfa.alphabet) {
        std::cout << symbol << " "; 
    }
    std::cout << "}" << std::endl;

    std::cout << "Transition Function:" << std::endl;
    for (const auto& [key, value] : dfa.transition_function) {
        std::cout << "\t(" << key.state << ", " << key.input << ") --> " << value << std::endl; 
    }
    std::cout << std::endl;
}

DFA createDFA(std::string regex) {
    NFA nfa = convertToNFA(convertToPostfix(regex));
    DFA dfa = convertToDFA(nfa);
    return minimizeDFA(dfa);
}

int main(int argc, char **argv) {

    std::string regex;
    std::cout << "Please provide a Regular Expression: ";
    getline(std::cin, regex);
    std::cout << "Constructing Finite Automaton for the regular expression: " << regex << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << std::endl;

    std::queue<char> postfix_regex = convertToPostfix(regex);

    NFA nfa = convertToNFA(postfix_regex);
    DFA dfa = convertToDFA(nfa);
    DFA min_dfa = minimizeDFA(dfa);

    std::cout << "NFA: " << std::endl;
    printNFA(nfa);
    std::cout << "DFA: " << std::endl;
    printDFA(dfa);
    std::cout << "Minimal DFA: " << std::endl;
    printDFA(min_dfa);
    std::cout << std::endl;

    std::string word;
    while(true) {
        std::cout << "Input: ";
        getline(std::cin, word);

        bool accepted = min_dfa.run(word);

        if(accepted) {
            std::cout << "Result: True" << std::endl;
            std::cout << "[" << word << "] is in the language of Regular Expression [" << regex << "]" << std::endl; 
        }
        else {
            std::cout << "Result: False" << std::endl;
            std::cout << "[" << word << "] is not in the language of Regular Expression [" << regex << "]" << std::endl;
        }
        std::cout << std:: endl;
    }

    return 0;
}