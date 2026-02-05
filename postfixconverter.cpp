#include <string>
#include "postfixconverter.h"

std::string addExplicitConcat(std::string regexStr) {
    std::string explicitConcatStr = "";
    for(std::string::size_type i = 0; i < regexStr.size(); i++) {
        char symbol = regexStr[i];
        explicitConcatStr = explicitConcatStr + symbol;
        
        bool canConcatFirst = symbol != '(' && symbol != '+';
        bool canConcatSecond = i < regexStr.size()-1 && regexStr[i+1] != ')' && regexStr[i+1] != '+' && regexStr[i+1] != '*';
        if(canConcatFirst && canConcatSecond) {
            explicitConcatStr = explicitConcatStr + "?";
        }

    }

    return explicitConcatStr;
}

