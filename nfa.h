#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>
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

typedef std::unordered_map<state_input_pair, std::vector<int32_t>, hasher> transition_function_t;

struct NFA {
    int32_t initialState;
    int32_t acceptingState;
    std::unordered_set<char> alphabet;
    transition_function_t transitionFunction;
};