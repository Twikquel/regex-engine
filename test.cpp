#include <iostream>
#include <cassert>
#include "postfixconverter.h"
#include "regex_to_nfa.h"
#include "nfa_to_dfa.h"


std::string queueToString(std::queue<char> queue) {
    std::string str = "";
    while(!queue.empty()) {
        char symbol = queue.front();
        queue.pop();

        str += symbol;
    }

    return str;
}

DFA createDFA(std::string regex) {
    return convertToDFA(convertToNFA(convertToPostfix(regex)));
}

void test_precedence() {
    assert(higherEqualPrecedence('*', '*') == true);
    assert(higherEqualPrecedence('*', '?') == true);
    assert(higherEqualPrecedence('*', '+') == true);

    assert(higherEqualPrecedence('?', '*') == false);
    assert(higherEqualPrecedence('?', '?') == true);
    assert(higherEqualPrecedence('?', '+') == true);

    assert(higherEqualPrecedence('+', '*') == false);
    assert(higherEqualPrecedence('+', '?') == false);
    assert(higherEqualPrecedence('+', '+') == true);
}

void test_regex_str_to_queue() {
    assert(queueToString(regexAsQueue("")) == "");
    assert(queueToString(regexAsQueue("ab")) == "a?b");
    assert(queueToString(regexAsQueue("(a+b)(a+b)")) == "(a+b)?(a+b)");
    assert(queueToString(regexAsQueue("a(a+b)")) == "a?(a+b)");
    assert(queueToString(regexAsQueue("a*(a+b)")) == "a*?(a+b)");
    assert(queueToString(regexAsQueue("(a+b)a")) == "(a+b)?a");
    assert(queueToString(regexAsQueue("(a+b)*a")) == "(a+b)*?a");
    assert(queueToString(regexAsQueue("a*b")) == "a*?b");
    assert(queueToString(regexAsQueue("a*b(ab+b*)abc")) == "a*?b?(a?b+b*)?a?b?c");
}

void test_postfix_conversion() {
    assert(queueToString(convertToPostfix("")) == "");
    assert(queueToString(convertToPostfix("a")) == "a");
    assert(queueToString(convertToPostfix("a*")) == "a*");
    assert(queueToString(convertToPostfix("ab")) == "ab?");
    assert(queueToString(convertToPostfix("a+b")) == "ab+");

    assert(queueToString(convertToPostfix("a*b")) == "a*b?");
    assert(queueToString(convertToPostfix("ab*")) == "ab*?");
    assert(queueToString(convertToPostfix("a*+b")) == "a*b+");
    assert(queueToString(convertToPostfix("a+b*")) == "ab*+");
    assert(queueToString(convertToPostfix("(ab)*")) == "ab?*");
    assert(queueToString(convertToPostfix("(a+b)*")) == "ab+*");

    assert(queueToString(convertToPostfix("ab+c")) == "ab?c+");
    assert(queueToString(convertToPostfix("a+bc")) == "abc?+");
    assert(queueToString(convertToPostfix("a(b+c)")) == "abc+?");
    assert(queueToString(convertToPostfix("(a+b)c")) == "ab+c?");
    
    assert(queueToString(convertToPostfix("a**+b")) == "a**b+");
    assert(queueToString(convertToPostfix("abc")) == "ab?c?");
    assert(queueToString(convertToPostfix("a(bc)")) == "abc??");
    assert(queueToString(convertToPostfix("a+b+c")) == "ab+c+");
    assert(queueToString(convertToPostfix("a+(b+c)")) == "abc++");
}

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

void test_dfa() {
    DFA empty_dfa = createDFA("");
    assert(empty_dfa.run(""));

    assert(!empty_dfa.run("a"));


    DFA unit_dfa = createDFA("a");
    assert(unit_dfa.run("a"));

    assert(!unit_dfa.run(""));
    assert(!unit_dfa.run("b"));


    DFA union_dfa = createDFA("a+b");
    assert(union_dfa.run("a"));
    assert(union_dfa.run("b"));

    assert(!union_dfa.run(""));
    assert(!union_dfa.run("c"));
    assert(!union_dfa.run("aa"));
    assert(!union_dfa.run("bb"));
    assert(!union_dfa.run("ab"));
    assert(!union_dfa.run("ba"));


    DFA concat_dfa = createDFA("ab");
    assert(concat_dfa.run("ab"));

    assert(!concat_dfa.run(""));
    assert(!concat_dfa.run("a"));
    assert(!concat_dfa.run("b"));
    assert(!concat_dfa.run("c"));
    assert(!concat_dfa.run("aba"));
    assert(!concat_dfa.run("abb"));

    DFA kleene_star_dfa = createDFA("a*");
    assert(kleene_star_dfa.run(""));
    assert(kleene_star_dfa.run("a"));
    assert(kleene_star_dfa.run("aa"));
    assert(kleene_star_dfa.run("aaaaaaaaaaa"));

    assert(!kleene_star_dfa.run("b"));
    assert(!kleene_star_dfa.run("baaaaaaaaa"));
    assert(!kleene_star_dfa.run("aaaabaaaaa"));
    assert(!kleene_star_dfa.run("aaaaaaab"));
}

int main() {
    test_precedence();
    test_regex_str_to_queue();
    test_postfix_conversion();

    test_empty_nfa();
    test_unit_nfa();
    test_union_nfa();
    test_concat_nfa();
    test_kleene_star_nfa();

    test_dfa();

    std::cout << "All tests passed" << std::endl;
} 
