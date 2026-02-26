#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdint>

struct state_input_pair {
    int32_t state;
    char input;

    bool operator== (const state_input_pair& other) const {
        return state == other.state && input == other.input;
    } 
};

struct hasher {
    size_t operator()(const state_input_pair& p) const
    {
        // Combine hashes of x and y using the bitwise XOR
        return std::hash<int>()(p.state) ^ (std::hash<int>()(p.input) << 1);
    }
};

typedef std::unordered_map<state_input_pair, std::unordered_set<int32_t>, hasher> nfa_transition_function_t;
typedef std::unordered_map<state_input_pair, int32_t, hasher> dfa_transition_function_t;

struct NFA {
    int32_t initialState;
    int32_t acceptingState;
    std::unordered_set<char> alphabet;
    nfa_transition_function_t transitionFunction;
};

struct DFA {
    int32_t initialState;
    std::unordered_set<int32_t> acceptingStates;
    std::unordered_set<char> alphabet;
    dfa_transition_function_t transitionFunction;

    bool run(std::string word) {
        int32_t currentState = initialState;

        for(char symbol : word) {
            if(transitionFunction.count({currentState, symbol}) == 1) {
                currentState = transitionFunction[{currentState, symbol}];
            }
            else return false;
        }

        return acceptingStates.count(currentState) == 1;
    }
};