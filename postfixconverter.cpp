#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include "postfixconverter.h"

bool isOperator(char symbol) {
    return symbol == '?' || symbol == '+' || symbol == '*';
}

bool higherEqualPrecedence(char op, char otherOp) {
    switch (op)
    {
    case '*':
        return true;
    case '?':
        return otherOp == '?' || otherOp == '+';
    case '+':
        return otherOp == '+';
    default:
        return false;
    }
}

std::queue<char> RegexAsQueue(std::string regexStr) {
    std::queue<char> regexQueue;
    for(std::string::size_type i = 0; i < regexStr.size(); i++) {
        char symbol = regexStr[i];
        regexQueue.push(symbol);
        
        //Add explicit concatenate operator
        bool canConcatFirst = symbol != '(' && symbol != '+';
        bool canConcatSecond = i < regexStr.size()-1 && regexStr[i+1] != ')' && regexStr[i+1] != '+' && regexStr[i+1] != '*';
        if(canConcatFirst && canConcatSecond) {
            regexQueue.push('?');
        }
    }

    return regexQueue;
}

std::queue<char> convertToPostfix(std::string regexStr) {
    std::queue<char> regexQueue = RegexAsQueue(regexStr);
    
    //Shunting-Yard algorithm
    std::queue<char> postfixQueue;
    std::stack<char> operatorStack;

    while (!regexQueue.empty()) {
        char symbol = regexQueue.front();
        regexQueue.pop();


        if(isOperator(symbol)) {
            if(operatorStack.empty() || operatorStack.top() == '(') {
                operatorStack.push(symbol);
            }
            else {
                while(!operatorStack.empty()) {
                    char op = operatorStack.top();

                    if(higherEqualPrecedence(op, symbol)) {
                        postfixQueue.push(op);
                        operatorStack.pop();
                    }
                    else {
                        break;
                    }
                }
                operatorStack.push(symbol);
            }
        }
        else if(symbol == '(') {
            operatorStack.push(symbol);
        }
        else if(symbol == ')') {
            bool openParFound = false;
            while(!openParFound) {
                char op = operatorStack.top();
                operatorStack.pop();

                if(op == '(') {
                    openParFound = true;
                }
                else {
                    postfixQueue.push(op);
                }
            }
        }
        else {
            postfixQueue.push(symbol);
        }
    }
    while(!operatorStack.empty()) {
        char op = operatorStack.top();
        postfixQueue.push(op);
        operatorStack.pop();
    }

    return postfixQueue;
}

