#include <queue>
#include <stack>

#include "automatons.h"
#include "regex_to_nfa.h"

int32_t nfa_state_counter = 0;

void reunordered_setNFAStateCounter() {
    nfa_state_counter = 0;
}

NFA convertToNFA(std::queue<char> regexQueue) {

    std::stack<NFA> nfaStack;

    if(regexQueue.empty()) {
        return createEmptyNFA();
    }

    while(!regexQueue.empty()) {
        char symbol = regexQueue.front();
        regexQueue.pop();

        switch (symbol)
        {
            case ' ': {
                NFA nfa = createEmptyNFA();
                nfaStack.push(nfa);
                break;
            }
            case '+': {
                NFA nfa2 = nfaStack.top();
                nfaStack.pop();
                NFA nfa1 = nfaStack.top();
                nfaStack.pop();

                NFA nfa = unionNFA(nfa1, nfa2);
                nfaStack.push(nfa);
                break;
            }
            case '?': {
                NFA nfa2 = nfaStack.top();
                nfaStack.pop();
                NFA nfa1 = nfaStack.top();
                nfaStack.pop();

                NFA nfa = concatNFA(nfa1, nfa2);
                nfaStack.push(nfa);
                break;
            }
            case '*': {
                NFA nfa1 = nfaStack.top();
                nfaStack.pop();

                NFA nfa = kleeneStarNFA(nfa1);
                nfaStack.push(nfa);
                break;
            }      
            default: {
                NFA nfa = createUnitNFA(symbol);
                nfaStack.push(nfa);
                break;
            }
        }
    }

    return nfaStack.top();
}

NFA createUnitNFA(char symbol) {
    int32_t initialState = nfa_state_counter++;
    int32_t acceptingState = nfa_state_counter++;
    std::unordered_set<char> alphabet = {symbol};

    state_input_pair transitionInput = {initialState, symbol};
    nfa_transition_function_t transitionFunction = {{transitionInput, {acceptingState}}};

    return {initialState, acceptingState, alphabet, transitionFunction};
}

NFA createEmptyNFA() {
    int32_t initialState = nfa_state_counter++;
    int32_t acceptingState = nfa_state_counter++;
    std::unordered_set<char> alphabet = {};

    state_input_pair transitionInput = {initialState, ' '};
    nfa_transition_function_t transitionFunction = {{transitionInput, {acceptingState}}};

    return {initialState, acceptingState, alphabet, transitionFunction};
}

 NFA unionNFA(NFA nfa1, NFA nfa2) {
    int32_t initialState = nfa_state_counter++;
    int32_t acceptingState = nfa_state_counter++;

    std::unordered_set<char> alphabet = nfa1.alphabet;
    alphabet.merge(nfa2.alphabet);

    nfa_transition_function_t transitionFunction = nfa1.transitionFunction;
    transitionFunction.merge(nfa2.transitionFunction);
    //transition_function_t transitionFunction = merge(nfa1.transitionFunction, nfa2.transitionFunction);
    
    //Add lambda transitions to initial state of nfa1 and nfa2
    transitionFunction[{state: initialState, input: ' '}] = {nfa1.initialState, nfa2.initialState};

    //Add lambda transitions from accepting states of nfa1 and nfa2 to new accepting state
    transitionFunction[{state: nfa1.acceptingState, input: ' '}] = {acceptingState};
    transitionFunction[{state: nfa2.acceptingState, input: ' '}] = {acceptingState};

    return {initialState, acceptingState, alphabet, transitionFunction};
}

NFA concatNFA(NFA nfa1, NFA nfa2) {
    int32_t initialState = nfa1.initialState;
    int32_t acceptingState = nfa2.acceptingState;

    std::unordered_set<char> alphabet = nfa1.alphabet;
    alphabet.merge(nfa2.alphabet);

    nfa_transition_function_t transitionFunction = nfa1.transitionFunction;
    transitionFunction.merge(nfa2.transitionFunction);
    //transition_function_t transitionFunction = merge(nfa1.transitionFunction, nfa2.transitionFunction);

    //Add lambda transition from accepting state of nfa1 to initial state of nfa2
    transitionFunction[{state: nfa1.acceptingState, input: ' '}] = {nfa2.initialState};

    return {initialState, acceptingState, alphabet, transitionFunction};
}


NFA kleeneStarNFA(NFA nfa) {
    int32_t initialState = nfa_state_counter++;
    int32_t acceptingState = nfa_state_counter++;
    std::unordered_set<char> alphabet = nfa.alphabet;
    nfa_transition_function_t transitionFunction = nfa.transitionFunction;

    transitionFunction[{state: initialState, input: ' '}] = {nfa.initialState, acceptingState};
    transitionFunction[{state: nfa.acceptingState, input: ' '}] = {nfa.initialState, acceptingState};

    return {initialState, acceptingState, alphabet, transitionFunction};
}
