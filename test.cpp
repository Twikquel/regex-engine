#include <iostream>
#include <cassert>
#include "postfixconverter.h"
#include "regex_to_nfa.h"

std::string queueToString(std::queue<char> queue) {
    std::string str = "";
    while(!queue.empty()) {
        char symbol = queue.front();
        queue.pop();

        str += symbol;
    }

    return str;
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
    reunordered_setNFAStateCounter();
    NFA emptyNFA = convertToNFA(convertToPostfix(""));

    assert(emptyNFA.initialState == 0);
    assert(emptyNFA.acceptingState == 1);

    std::unordered_set<char> alphabet = {};
    assert(emptyNFA.alphabet == alphabet);

    nfa_transition_function_t tf = emptyNFA.transitionFunction;
    
    std::unordered_set<int32_t> toStates = { 1 };
    assert((tf[{0, ' '}] == toStates));
}

void test_unit_nfa() {
    reunordered_setNFAStateCounter();
    NFA unitNFA = convertToNFA(convertToPostfix("a"));

    assert(unitNFA.initialState == 0);
    assert(unitNFA.acceptingState == 1);

    std::unordered_set<char> alphabet = { 'a' };
    assert(unitNFA.alphabet == alphabet);

    nfa_transition_function_t tf = unitNFA.transitionFunction;
    
    std::unordered_set<int32_t> toStates = { 1 };
    assert((tf[{0, 'a'}] == toStates));
}

void test_union_nfa() {
    reunordered_setNFAStateCounter();
    NFA union_nfa = convertToNFA(convertToPostfix("a+b"));

    assert(union_nfa.initialState == 4);
    assert(union_nfa.acceptingState == 5);

    std::unordered_set<char> alphabet = {'a','b'};
    assert(union_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = union_nfa.transitionFunction;
    
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
    reunordered_setNFAStateCounter();
    NFA concat_nfa = convertToNFA(convertToPostfix("ab"));

    assert(concat_nfa.initialState == 0);
    assert(concat_nfa.acceptingState == 3);

    std::unordered_set<char> alphabet = {'a','b'};
    assert(concat_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = concat_nfa.transitionFunction;
    
    std::unordered_set<int32_t> s1 = {1};
    assert((tf[{0, 'a'}] == s1));
    std::unordered_set<int32_t> s2 = {2};
    assert((tf[{1, ' '}] == s2));
    std::unordered_set<int32_t> s3 = {3};
    assert((tf[{2, 'b'}] == s3));
}

void test_kleene_star_nfa() {
    reunordered_setNFAStateCounter();
    NFA kleene_star_nfa = convertToNFA(convertToPostfix("a*"));

    assert(kleene_star_nfa.initialState == 2);
    assert(kleene_star_nfa.acceptingState == 3);

    std::unordered_set<char> alphabet = {'a'};
    assert(kleene_star_nfa.alphabet == alphabet);

    nfa_transition_function_t tf = kleene_star_nfa.transitionFunction;
    
    std::unordered_set<int32_t> s1 = {1};
    assert((tf[{0, 'a'}] == s1));
    std::unordered_set<int32_t> s03 = {0,3};
    assert((tf[{1, ' '}] == s03));
    assert((tf[{2, ' '}] == s03));
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


    std::cout << "All tests passed" << std::endl;
} 
