#pragma once

#include "nfa.h"
#include <queue>

void resetStateCounter();

NFA convertToNFA(std::queue<char> regex);

NFA createUnitNFA(char symbol);

NFA createEmptyNFA();

NFA unionNFA(NFA nfa1, NFA nfa2);

NFA concatNFA(NFA nfa1, NFA nfa2);

NFA kleeneStarNFA(NFA nfa);