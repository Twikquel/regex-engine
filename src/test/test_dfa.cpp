#include <cassert>
#include "postfixconverter.h"
#include "regex_to_nfa.h"
#include "nfa_to_dfa.h"

DFA createDFA(std::string regex) {
    NFA nfa = convertToNFA(convertToPostfix(regex));
    return convertToDFA(nfa);
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