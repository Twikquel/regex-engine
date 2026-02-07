#include <queue>
#include <stack>

#include "nfa.h"
#include "regex_to_nfa.h"

int32_t state_counter = 0;

NFA convertToNFA(std::queue<char> regexQueue) {

    std::stack<NFA> nfaStack;

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

transition_function_t merge(transition_function_t func1, const transition_function_t func2) {

    for(auto e : func2) {
        func1[e.first] = e.second;
    }

    return func1;
}

NFA createUnitNFA(char symbol) {
    int32_t initialState = state_counter++;
    int32_t acceptingState = state_counter++;
    std::unordered_set<char> alphabet = {symbol};

    state_input_pair transitionInput = {initialState, symbol};
    transition_function_t transitionFunction = {{transitionInput, {acceptingState}}};

    return {initialState, acceptingState, alphabet, transitionFunction};
}

NFA createEmptyNFA() {
    int32_t initialState = state_counter++;
    int32_t acceptingState = state_counter++;
    std::unordered_set<char> alphabet = {};

    state_input_pair transitionInput = {initialState, ' '};
    transition_function_t transitionFunction = {{transitionInput, {acceptingState}}};

    return {initialState, acceptingState, alphabet, transitionFunction};
}

 NFA unionNFA(NFA nfa1, NFA nfa2) {
    int32_t initialState = state_counter++;
    int32_t acceptingState = state_counter++;

    std::unordered_set<char> alphabet = nfa1.alphabet;
    alphabet.merge(nfa2.alphabet);

    transition_function_t transitionFunction = nfa1.transitionFunction;
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

    transition_function_t transitionFunction = nfa1.transitionFunction;
    transitionFunction.merge(nfa2.transitionFunction);
    //transition_function_t transitionFunction = merge(nfa1.transitionFunction, nfa2.transitionFunction);

    //Add lambda transition from accepting state of nfa1 to initial state of nfa2
    transitionFunction[{state: nfa1.acceptingState, input: ' '}] = {nfa2.initialState};

    return {initialState, acceptingState, alphabet, transitionFunction};
}


NFA kleeneStarNFA(NFA nfa) {
    int32_t initialState = state_counter++;
    int32_t acceptingState = state_counter++;
    std::unordered_set<char> alphabet = nfa.alphabet;
    transition_function_t transitionFunction = nfa.transitionFunction;

    transitionFunction[{state: initialState, input: ' '}] = {nfa.initialState, acceptingState};
    transitionFunction[{state: nfa.acceptingState, input: ' '}] = {nfa.initialState, acceptingState};

    return {initialState, acceptingState, alphabet, transitionFunction};
}
