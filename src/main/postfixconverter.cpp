#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include "postfixconverter.h"

bool isOperator(char symbol) {
    return symbol == '?' || symbol == '+' || symbol == '*';
}

bool higherEqualPrecedence(char op, char other_op) {
    switch (op)
    {
    case '*':
        return true;
    case '?':
        return other_op == '?' || other_op == '+';
    case '+':
        return other_op == '+';
    default:
        return false;
    }
}

std::queue<char> regexAsQueue(std::string regex_str) {
    std::queue<char> regex_queue;
    for(std::string::size_type i = 0; i < regex_str.size(); i++) {
        char symbol = regex_str[i];
        regex_queue.push(symbol);
        
        //Add explicit concatenate operator
        bool can_concat_first = symbol != '(' && symbol != '+';
        bool can_concat_second = i < regex_str.size()-1 && regex_str[i+1] != ')' && regex_str[i+1] != '+' && regex_str[i+1] != '*';
        if(can_concat_first && can_concat_second) {
            regex_queue.push('?');
        }
    }

    return regex_queue;
}

std::queue<char> convertToPostfix(std::string regex_str) {
    std::queue<char> regex_queue = regexAsQueue(regex_str);
    
    //Shunting-Yard algorithm
    std::queue<char> postfix_queue;
    std::stack<char> operator_stack;

    while (!regex_queue.empty()) {
        char symbol = regex_queue.front();
        regex_queue.pop();


        if(isOperator(symbol)) {
            if(operator_stack.empty() || operator_stack.top() == '(') {
                operator_stack.push(symbol);
            }
            else {
                while(!operator_stack.empty()) {
                    char op = operator_stack.top();

                    if(higherEqualPrecedence(op, symbol)) {
                        postfix_queue.push(op);
                        operator_stack.pop();
                    }
                    else {
                        break;
                    }
                }
                operator_stack.push(symbol);
            }
        }
        else if(symbol == '(') {
            operator_stack.push(symbol);
        }
        else if(symbol == ')') {
            bool open_parentheses_found = false;
            while(!open_parentheses_found) {
                char op = operator_stack.top();
                operator_stack.pop();

                if(op == '(') {
                    open_parentheses_found = true;
                }
                else {
                    postfix_queue.push(op);
                }
            }
        }
        else {
            postfix_queue.push(symbol);
        }
    }
    while(!operator_stack.empty()) {
        char op = operator_stack.top();
        postfix_queue.push(op);
        operator_stack.pop();
    }

    return postfix_queue;
}

