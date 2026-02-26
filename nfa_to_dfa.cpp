
#include <cstdint>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include "automatons.h"

struct set_hasher {
    size_t operator()(const std::unordered_set<int32_t>& set) const
    {
        size_t hash = 0;
        for(int32_t i : set) {
            hash = hash ^ std::hash<int>()(i);
        }
        return hash;
    }
};

std::unordered_set<int32_t> getLambdaClosure(NFA nfa, std::unordered_set<int32_t> states) {
    std::unordered_set<int32_t> closure = states;

    std::stack<int32_t> states_to_check;
    for(int32_t state : states) {
        states_to_check.push(state);
    }

    while(!states_to_check.empty()){
        int32_t nextState = states_to_check.top();
        states_to_check.pop();

        std::unordered_set<int32_t> to_states = nfa.transition_function[{nextState, ' '}];
        for(int32_t to_state : to_states) {
            if(closure.count(to_state) == 0) {
                states_to_check.push(to_state);
                closure.insert(to_state);
            }
        }
    }

    return closure;
}

std::unordered_set<int32_t> getReachableStates(NFA nfa, std::unordered_set<int32_t> states, char symbol) {
    std::unordered_set<int32_t> one_step_reachable = {};

    for(int32_t from_state : states) {
        for(int32_t to_state : nfa.transition_function[{from_state, symbol}]) {
            one_step_reachable.insert(to_state);
        }
    }

    return getLambdaClosure(nfa, one_step_reachable);
}

DFA convertToDFA(NFA nfa) {
    int32_t dfa_state_counter = 0;
    std::unordered_set<int32_t> dfa_initial_state = getLambdaClosure(nfa, {nfa.initial_state});
    std::unordered_map<std::unordered_set<int32_t>, int32_t, set_hasher> state_name_map = {{dfa_initial_state, dfa_state_counter++}};
    
    std::unordered_set<int32_t> states = {state_name_map[dfa_initial_state]};
    std::unordered_set<int32_t> accepting_states = {};
    dfa_transition_function_t transition_function = {};
    
    std::stack<std::unordered_set<int32_t>> unprocessed_states;
    unprocessed_states.push(dfa_initial_state);

    while(!unprocessed_states.empty()) {
        std::unordered_set<int32_t> unprocessed_state = unprocessed_states.top();
        unprocessed_states.pop();

        if(unprocessed_state.count(nfa.accepting_state) == 1) {
            accepting_states.insert(state_name_map[unprocessed_state]);
        }

        for(char symbol : nfa.alphabet) {
            std::unordered_set<int32_t> reachable_states = getReachableStates(nfa, unprocessed_state, symbol);
            
            if(!reachable_states.empty()) {
                if(state_name_map.count(reachable_states) == 0) {
                    state_name_map[reachable_states] = dfa_state_counter++;
                    states.insert(state_name_map[reachable_states]);
                    unprocessed_states.push(reachable_states);
                }

                transition_function[{state_name_map[unprocessed_state], symbol}] = state_name_map[reachable_states];
            }
        }
    }

    return {states, state_name_map[dfa_initial_state], accepting_states, nfa.alphabet, transition_function};
}