#include <cassert>
#include "postfixconverter.h"
#include "regex_to_nfa.h"

void test_empty_nfa() {
    resetNFAStateCounter();
    NFA empty_nfa = convertToNFA(convertToPostfix(""));

    assert(empty_nfa.initial_state == 0);
    assert(empty_nfa.accepting_state == 1);

    std::unordered_set<int32_t> states = {0,1};
    assert(empty_nfa.states == states);

    std::unordered_set<char> alphabet = {};
    assert(empty_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = empty_nfa.transition_function;
    
    std::unordered_set<int32_t> to_states = { 1 };
    assert((tf[{0, ' '}] == to_states));
}

void test_unit_nfa() {
    resetNFAStateCounter();
    NFA unit_nfa = convertToNFA(convertToPostfix("a"));

    assert(unit_nfa.initial_state == 0);
    assert(unit_nfa.accepting_state == 1);

    std::unordered_set<int32_t> states = {0,1};
    assert(unit_nfa.states == states);

    std::unordered_set<char> alphabet = { 'a' };
    assert(unit_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = unit_nfa.transition_function;
    
    std::unordered_set<int32_t> to_states = { 1 };
    assert((tf[{0, 'a'}] == to_states));
}

void test_union_nfa() {
    resetNFAStateCounter();
    NFA union_nfa = convertToNFA(convertToPostfix("a+b"));

    assert(union_nfa.initial_state == 4);
    assert(union_nfa.accepting_state == 5);

    std::unordered_set<int32_t> states = {0,1,2,3,4,5};
    assert(union_nfa.states == states);

    std::unordered_set<char> alphabet = {'a','b'};
    assert(union_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = union_nfa.transition_function;
    
    std::unordered_set<int32_t> s1 = {1};
    assert((tf[{0, 'a'}] == s1));
    std::unordered_set<int32_t> s3 = {3};
    assert((tf[{2, 'b'}] == s3));
    std::unordered_set<int32_t> s02 = {0, 2};
    assert((tf[{4, ' '}] == s02));
    std::unordered_set<int32_t> s5 = {5};
    assert((tf[{1, ' '}] == s5));
    assert((tf[{3, ' '}] == s5));
}

void test_concat_nfa() {
    resetNFAStateCounter();
    NFA concat_nfa = convertToNFA(convertToPostfix("ab"));

    assert(concat_nfa.initial_state == 0);
    assert(concat_nfa.accepting_state == 3);

    std::unordered_set<int32_t> states = {0,1,2,3};
    assert(concat_nfa.states == states);

    std::unordered_set<char> alphabet = {'a','b'};
    assert(concat_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = concat_nfa.transition_function;
    
    std::unordered_set<int32_t> s1 = {1};
    assert((tf[{0, 'a'}] == s1));
    std::unordered_set<int32_t> s2 = {2};
    assert((tf[{1, ' '}] == s2));
    std::unordered_set<int32_t> s3 = {3};
    assert((tf[{2, 'b'}] == s3));
}

void test_kleene_star_nfa() {
    resetNFAStateCounter();
    NFA kleene_star_nfa = convertToNFA(convertToPostfix("a*"));

    assert(kleene_star_nfa.initial_state == 2);
    assert(kleene_star_nfa.accepting_state == 3);

    std::unordered_set<int32_t> states = {0,1,2,3};
    assert(kleene_star_nfa.states == states);

    std::unordered_set<char> alphabet = {'a'};
    assert(kleene_star_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = kleene_star_nfa.transition_function;
    
    std::unordered_set<int32_t> s1 = {1};
    assert((tf[{0, 'a'}] == s1));
    std::unordered_set<int32_t> s03 = {0,3};
    assert((tf[{1, ' '}] == s03));
    assert((tf[{2, ' '}] == s03));
}

void test_nfa() {
    test_empty_nfa();
    test_unit_nfa();
    test_union_nfa();
    test_concat_nfa();
    test_kleene_star_nfa();
}
