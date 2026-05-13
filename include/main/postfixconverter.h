#pragma once

#include <queue>
#include <string>

bool isOperator(char symbol);

bool higherEqualPrecedence(char op, char other_op);

std::queue<char> regexAsQueue(std::string regex_str);

std::queue<char> convertToPostfix(std::string regex_str);