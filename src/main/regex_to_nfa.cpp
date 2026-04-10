#include <queue>
#include <stack>

#include "automatons.h"
#include "regex_to_nfa.h"

int32_t nfa_state_counter = 0;

void resetNFAStateCounter() {
    nfa_state_counter = 0;
}

NFA convertToNFA(std::queue<char> regex_queue) {

    std::stack<NFA> nfa_stack;

    if(regex_queue.empty()) {
        return createEmptyNFA();
    }

    while(!regex_queue.empty()) {
        char symbol = regex_queue.front();
        regex_queue.pop();

        switch (symbol)
        {
            case ' ': {
                NFA nfa = createEmptyNFA();
                nfa_stack.push(nfa);
                break;
            }
            case '+': {
                NFA nfa2 = nfa_stack.top();
                nfa_stack.pop();
                NFA nfa1 = nfa_stack.top();
                nfa_stack.pop();

                NFA nfa = unionNFA(nfa1, nfa2);
                nfa_stack.push(nfa);
                break;
            }
            case '?': {
                NFA nfa2 = nfa_stack.top();
                nfa_stack.pop();
                NFA nfa1 = nfa_stack.top();
                nfa_stack.pop();

                NFA nfa = concatNFA(nfa1, nfa2);
                nfa_stack.push(nfa);
                break;
            }
            case '*': {
                NFA nfa1 = nfa_stack.top();
                nfa_stack.pop();

                NFA nfa = kleeneStarNFA(nfa1);
                nfa_stack.push(nfa);
                break;
            }      
            default: {
                NFA nfa = createUnitNFA(symbol);
                nfa_stack.push(nfa);
                break;
            }
        }
    }

    return nfa_stack.top();
}

NFA createUnitNFA(char symbol) {
    int32_t initial_state = nfa_state_counter++;
    int32_t accepting_state = nfa_state_counter++;
    std::unordered_set<int32_t> states = {initial_state, accepting_state};
    std::unordered_set<char> alphabet = {symbol};

    state_input_pair transition_input = {initial_state, symbol};
    nfa_transition_function_t transition_function = {{transition_input, {accepting_state}}};

    return {states, initial_state, accepting_state, alphabet, transition_function};
}

NFA createEmptyNFA() {
    int32_t initial_state = nfa_state_counter++;
    int32_t accepting_state = nfa_state_counter++;
    std::unordered_set<int32_t> states = {initial_state, accepting_state};
    std::unordered_set<char> alphabet = {};

    state_input_pair transition_input = {initial_state, ' '};
    nfa_transition_function_t transition_function = {{transition_input, {accepting_state}}};

    return {states, initial_state, accepting_state, alphabet, transition_function};
}

 NFA unionNFA(const NFA& nfa1, const NFA& nfa2) {
    int32_t initial_state = nfa_state_counter++;
    int32_t accepting_state = nfa_state_counter++;
    
    std::unordered_set<int32_t> states = nfa1.states;
    for(int32_t state : nfa2.states) {
        states.insert(state);
    }
    states.insert(initial_state);
    states.insert(accepting_state);

    std::unordered_set<char> alphabet = nfa1.alphabet;
    for(char symbol : nfa2.alphabet) {
        alphabet.insert(symbol);
    }

    nfa_transition_function_t transition_function = nfa1.transition_function;
    for(const auto& [key, value] : nfa2.transition_function) {
        transition_function[key] = value;
    }
    
    //Add lambda transitions to initial state of nfa1 and nfa2
    transition_function[{state: initial_state, input: ' '}] = {nfa1.initial_state, nfa2.initial_state};

    //Add lambda transitions from accepting states of nfa1 and nfa2 to new accepting state
    transition_function[{state: nfa1.accepting_state, input: ' '}] = {accepting_state};
    transition_function[{state: nfa2.accepting_state, input: ' '}] = {accepting_state};

    return {states, initial_state, accepting_state, alphabet, transition_function};
}

NFA concatNFA(const NFA& nfa1, const NFA& nfa2) {
    int32_t initial_state = nfa1.initial_state;
    int32_t accepting_state = nfa2.accepting_state;

    std::unordered_set<int32_t> states = nfa1.states;
    for(int32_t state : nfa2.states) {
        states.insert(state);
    }
    states.insert(initial_state);
    states.insert(accepting_state);

    std::unordered_set<char> alphabet = nfa1.alphabet;
    for(char symbol : nfa2.alphabet) {
        alphabet.insert(symbol);
    }

    nfa_transition_function_t transition_function = nfa1.transition_function;
    for(const auto& [key, value] : nfa2.transition_function) {
        transition_function[key] = value;
    }

    //Add lambda transition from accepting state of nfa1 to initial state of nfa2
    transition_function[{state: nfa1.accepting_state, input: ' '}] = {nfa2.initial_state};

    return {states, initial_state, accepting_state, alphabet, transition_function};
}


NFA kleeneStarNFA(const NFA& nfa) {
    int32_t initial_state = nfa_state_counter++;
    int32_t accepting_state = nfa_state_counter++;

    std::unordered_set<int32_t> states = nfa.states;
    states.insert(initial_state);
    states.insert(accepting_state);

    std::unordered_set<char> alphabet = nfa.alphabet;
    nfa_transition_function_t transition_function = nfa.transition_function;

    transition_function[{state: initial_state, input: ' '}] = {nfa.initial_state, accepting_state};
    transition_function[{state: nfa.accepting_state, input: ' '}] = {nfa.initial_state, accepting_state};

    return {states, initial_state, accepting_state, alphabet, transition_function};
}
