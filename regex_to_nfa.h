#pragma once

#include "nfa.h"
#include <queue>

transition_function_t merge(transition_function_t func1, const transition_function_t func2);

NFA convertToNFA(std::queue<char> regex);

NFA createUnitNFA(char symbol);

NFA createEmptyNFA();

NFA unionNFA(NFA nfa1, NFA nfa2);

NFA concatNFA(NFA nfa1, NFA nfa2);

NFA kleeneStarNFA(NFA nfa);