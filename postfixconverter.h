#pragma once

#include <queue>
#include <string>

bool isOperator(char symbol);

bool higherEqualPrecedence(char op, char otherOp);

std::queue<char> regexAsQueue(std::string regexStr);

std::queue<char> convertToPostfix(std::string regexStr);