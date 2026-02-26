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
    std::unordered_set<int32_t> states;
    int32_t initial_state;
    int32_t accepting_state;
    std::unordered_set<char> alphabet;
    nfa_transition_function_t transition_function;
};

struct DFA {
    std::unordered_set<int32_t> states;
    int32_t initial_state;
    std::unordered_set<int32_t> accepting_states;
    std::unordered_set<char> alphabet;
    dfa_transition_function_t transition_function;

    bool run(std::string word) {
        int32_t current_state = initial_state;

        for(char symbol : word) {
            if(transition_function.count({current_state, symbol}) == 1) {
                current_state = transition_function[{current_state, symbol}];
            }
            else return false;
        }

        return accepting_states.count(current_state) == 1;
    }
};