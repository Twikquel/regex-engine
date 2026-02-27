#include <map>
#include <iostream>
#include <vector>
#include <stack>
#include "automatons.h"


void completeDFA(DFA& dfa) {
    int32_t max_state = 0;
    for(int32_t state : dfa.states) {
        if(max_state < state) {
            max_state = state;
        }
    }
    
    int32_t trap_state = max_state + 1;

    bool need_trap_state = false;
    for(int32_t state : dfa.states) {
        for(char symbol : dfa.alphabet) {
            if(dfa.transition_function.count({state, symbol}) == 0) {
                dfa.transition_function[{state, symbol}] = trap_state;
                need_trap_state = true;
            }
        }
    }

    if(need_trap_state) {
        dfa.states.insert(trap_state);
        for(char symbol : dfa.alphabet) {
            dfa.transition_function[{trap_state, symbol}] = trap_state;
        }
    }
}

std::map<std::pair<int32_t, int32_t>, bool> computeDistinguishability(DFA& dfa) {
    std::map<std::pair<int32_t, int32_t>, bool> indistinguishable;
    
    for(int32_t state1 : dfa.states) {
        for(int32_t state2 : dfa.states) {
            indistinguishable[{state1, state2}] = dfa.accepting_states.count(state1) == dfa.accepting_states.count(state2);
        }
    }

    bool updated = true;
    while(updated) {
        updated = false;

        for(int32_t state1 : dfa.states) {
            for(int32_t state2 : dfa.states) {
                for(char symbol : dfa.alphabet) {
                    int32_t successor1 = dfa.transition_function[{state1, symbol}];
                    int32_t successor2 = dfa.transition_function[{state2, symbol}];
                    
                    if(indistinguishable[{state1, state2}] && !indistinguishable[{successor1, successor2}]) {
                        indistinguishable[{state1, state2}] = false;
                        updated = true;
                    }
                }
            }
        }
    }

    return indistinguishable;
}

std::unordered_set<int32_t> dfs(int32_t state, std::unordered_set<int32_t>& states, std::map<std::pair<int32_t, int32_t>, bool>& indistinguishable) {
    std::unordered_set<int32_t> equivalent_states = {state};
    std::stack<int32_t> states_to_check;
    states_to_check.push(state);

    while(!states_to_check.empty()) {
        int32_t next_state = states_to_check.top();
        states_to_check.pop();

        for(int32_t other : states) {
            if(indistinguishable[{next_state, other}] && equivalent_states.count(other) == 0) {
                equivalent_states.insert(other);
                states_to_check.push(other);
            }
        }
    }

    return equivalent_states;
}

DFA minimizeDFA(DFA dfa) {
    completeDFA(dfa);
    std::map<std::pair<int32_t, int32_t>, bool> indistinguishable = computeDistinguishability(dfa);

    std::vector<std::unordered_set<int32_t>> equivalence_classes = {};
    std::unordered_set<int32_t> processed_states = {};

    for(int32_t state : dfa.states) {
        if(processed_states.count(state) == 1) {
            continue;
        }
        std::unordered_set<int32_t> equivalence_class = dfs(state, dfa.states, indistinguishable);
        equivalence_classes.push_back(equivalence_class);

        for(int32_t state : equivalence_class) {
            processed_states.insert(state);
        }
    }

    std::unordered_set<int32_t> min_dfa_states = {};
    int32_t min_dfa_initial_state;
    std::unordered_set<int32_t> min_dfa_accepting_states = {};

    std::map<int32_t, int32_t> eqclass_to_represant_map;

    for(int32_t i = 0; i < (int32_t) equivalence_classes.size(); i++) {
        //Find states of minimal dfa
        min_dfa_states.insert(i);

        //Find initial state for minimal dfa
        if(equivalence_classes[i].count(dfa.initial_state) == 1) {
            min_dfa_initial_state = i;
        }

        //Find accepting states of minimal dfa
        if(dfa.accepting_states.count(*equivalence_classes[i].begin()) == 1) {
            min_dfa_accepting_states.insert(i);
        }

        //Setup mapping from member states of eq classes to representing state in minimal dfa
        for(int32_t state : equivalence_classes[i]) {
            eqclass_to_represant_map[state] = i;
        }
    }

    dfa_transition_function_t min_dfa_tf;
    for (const auto& [key, to_state] : dfa.transition_function) {
        int32_t from_state = key.state;
        char symbol = key.input;

        min_dfa_tf[{eqclass_to_represant_map[from_state], symbol}] = eqclass_to_represant_map[to_state];
    }

    return {min_dfa_states, min_dfa_initial_state, min_dfa_accepting_states, dfa.alphabet, min_dfa_tf};
}