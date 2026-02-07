#include "postfixconverter.h"
#include "regex_to_nfa.h"

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

int main(int32_t argc, char *argv[]) {
    std::queue<char> postfixRegex = convertToPostfix(argv[1]);

    NFA nfa = convertToNFA(postfixRegex);

    printNFA(nfa);
    /* NFA nfa1 = createUnitNFA('a');
    printNFA(nfa1);

    NFA nfa2 = createUnitNFA('b');
    printNFA(nfa2);

    NFA nfa3 = unionNFA(nfa1, nfa2);
    printNFA(nfa3);

    NFA nfa4 = concatNFA(nfa1, nfa2);
    printNFA(nfa4);

    NFA nfa5 = kleeneStarNFA(nfa1);
    printNFA(nfa5); */

    return 0;
}