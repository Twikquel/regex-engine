
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

    std::stack<int32_t> statesToCheck;
    for(int32_t state : states) {
        statesToCheck.push(state);
    }

    while(!statesToCheck.empty()){
        int32_t nextState = statesToCheck.top();
        statesToCheck.pop();

        std::unordered_set<int32_t> toStates = nfa.transitionFunction[{nextState, ' '}];
        for(int32_t toState : toStates) {
            if(closure.count(toState) == 0) {
                statesToCheck.push(toState);
                closure.insert(toState);
            }
        }
    }

    return closure;
}

std::unordered_set<int32_t> getReachableStates(NFA nfa, std::unordered_set<int32_t> states, char symbol) {
    std::unordered_set<int32_t> oneStepReachable = {};

    for(int32_t fromState : states) {
        for(int32_t toState : nfa.transitionFunction[{fromState, symbol}]) {
            oneStepReachable.insert(toState);
        }
    }

    return getLambdaClosure(nfa, oneStepReachable);
}

DFA convertToDFA(NFA nfa) {
    int32_t dfa_state_counter = 0;
    std::unordered_set<int32_t> dfaInitialState = getLambdaClosure(nfa, {nfa.initialState});
    std::unordered_map<std::unordered_set<int32_t>, int32_t, set_hasher> stateNameMap = {{dfaInitialState, dfa_state_counter++}};
    
    std::unordered_set<int32_t> states = {};
    std::unordered_set<int32_t> acceptingStates = {};
    dfa_transition_function_t transitionFunction = {};
    
    std::stack<std::unordered_set<int32_t>> unprocessed_states;
    unprocessed_states.push(dfaInitialState);

    while(!unprocessed_states.empty()) {
        std::unordered_set<int32_t> unprocessed_state = unprocessed_states.top();
        unprocessed_states.pop();

        if(unprocessed_state.count(nfa.acceptingState) == 1) {
            acceptingStates.insert(stateNameMap[unprocessed_state]);
        }

        for(char symbol : nfa.alphabet) {
            std::unordered_set<int32_t> reachableStates = getReachableStates(nfa, unprocessed_state, symbol);
            
            if(!reachableStates.empty()) {
                if(stateNameMap.count(reachableStates) == 0) {
                    stateNameMap[reachableStates] = dfa_state_counter++;
                    states.insert(stateNameMap[reachableStates]);
                    unprocessed_states.push(reachableStates);
                }

                transitionFunction[{stateNameMap[unprocessed_state], symbol}] = stateNameMap[reachableStates];
            }
        }
    }

    return {states, stateNameMap[dfaInitialState], acceptingStates, nfa.alphabet, transitionFunction};
}