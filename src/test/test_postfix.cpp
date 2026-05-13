#include <cassert>
#include "postfixconverter.h"

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

void test_postfix() {
    test_precedence();
    test_regex_str_to_queue();
    test_postfix_conversion();
}