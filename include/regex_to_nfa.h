#pragma once

#include <queue>
#include "automatons.h"

void resetNFAStateCounter();

NFA convertToNFA(std::queue<char> regex);

NFA createUnitNFA(char symbol);

NFA createEmptyNFA();

NFA unionNFA(const NFA& nfa1, const NFA& nfa2);

NFA concatNFA(const NFA& nfa1, const NFA& nfa2);

NFA kleeneStarNFA(const NFA& nfa);