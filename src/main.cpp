#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <stack>

int isValidInput(std::string& input, std::set<char>& validCharacters) {
    for (char c : input) {
        if (validCharacters.find(c) == validCharacters.end()) {
            return false;
        }
    }
    return true;
}

std::set<char> getCharacterSet(const std::string characters) {
    std::set<char> validCharacters;

    for (char c : characters) {
        validCharacters.insert(c);
    }
    return validCharacters;
}

std::set<char> validCharacters{ getCharacterSet(".0123456789+-*/^()") };
std::set<char> numbers{ getCharacterSet(".01234567890") };
std::set<char> operations{ getCharacterSet("+-*/^") };
std::set<char> braces{ getCharacterSet("()") };

std::vector<std::string> tokenise(const std::string& input) {
    std::vector<std::string> tokens;
    std::string currentToken;

    for (char c : input) {
        if (numbers.find(c) != numbers.end()) {
            currentToken += c;
        }
        else {
            if (currentToken != "") {
                tokens.push_back(currentToken);
                currentToken = "";
            }
            tokens.push_back(std::string(1, c));
        }
    }

    if (currentToken != "") { tokens.push_back(currentToken); } ;
    return tokens;
}

template <typename T>
void printVector(std::vector<T> v, int begin=0, int end=-1) {
    if (end == -1) { end = v.size()-1; }
    for (int i=begin; i<=end; i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << std::endl;
}


void clearVector(std::vector<std::string>& v, int begin, int end) {
    for (int i = begin; i <= end; i++) {
        v[i] = "";
    }
}

int findNextNumber(std::vector<std::string>& v, unsigned int index) {
    while (index < v.size()) {
        if (v[index] == "") {
            continue;
        }
        if (numbers.find(v[index][0]) != numbers.end()) {
            return index;
        }
        ++index;
    }
    throw("No next number found\n");
}

long double solve(std::vector<std::string> tokens, int begin=0, int end=-1) {
    if (end == -1) { end = tokens.size()-1; }

    //braces
    std::stack<int> openedBraces;

    for (int i = begin; i <= end; i++) {
        if (tokens[i] == "(") {
            openedBraces.push(i);
        }
        else if (tokens[i] == ")") {
            tokens[openedBraces.top()] = std::to_string(solve(tokens, openedBraces.top()+1, i-1));
            clearVector(tokens, openedBraces.top()+1, i);
            openedBraces.pop();
        }
    }

    //powers (and roots)
    int lastNumberIndex{ 0 };
    int nextNumberIndex{ 0 };
    for (int i = 0; i <= end; i++) {
        if (tokens[i] != "" && numbers.find(tokens[i][0]) != numbers.end()) {
            lastNumberIndex = i;
        }

        if (tokens[i] == "^") {
            nextNumberIndex = findNextNumber(tokens, i);
            tokens[lastNumberIndex] = std::to_string(std::pow(stold(tokens[lastNumberIndex]), stold(tokens[nextNumberIndex])));
            i = nextNumberIndex;
        }
    }

    //multiplication and division
    for (int i = begin; i <= end; i++) {
        if (tokens[i].length() > 0 && numbers.find(tokens[i][0]) != numbers.end()) {
            lastNumberIndex = i;
        }

        if (tokens[i] == "*") {
            nextNumberIndex = findNextNumber(tokens, i);
            tokens[lastNumberIndex] = std::to_string(stold(tokens[lastNumberIndex]) * stold(tokens[nextNumberIndex]));
            clearVector(tokens, lastNumberIndex + 1, nextNumberIndex);
            i = nextNumberIndex;
        }
        else if (tokens[i] == "/") {
            nextNumberIndex = findNextNumber(tokens, i);
            tokens[lastNumberIndex] = std::to_string(stold(tokens[lastNumberIndex]) / stold(tokens[nextNumberIndex]));
            clearVector(tokens, lastNumberIndex+1, nextNumberIndex);
            i = nextNumberIndex;
        }
    }

    //sum and addition
    for (int i = begin; i <= end; i++) {
        if (tokens[i].length() > 0 && numbers.find(tokens[i][0]) != numbers.end()) {
            lastNumberIndex = i;
        }

        if (tokens[i] == "+") {
            nextNumberIndex = findNextNumber(tokens, i);
            tokens[lastNumberIndex] = std::to_string(stold(tokens[lastNumberIndex]) + stold(tokens[nextNumberIndex]));
            clearVector(tokens, lastNumberIndex + 1, nextNumberIndex);
            i = nextNumberIndex;
        }
        else if (tokens[i] == "-") {
            nextNumberIndex = findNextNumber(tokens, i);
            tokens[lastNumberIndex] = std::to_string(stold(tokens[lastNumberIndex]) - stold(tokens[nextNumberIndex]));
            clearVector(tokens, lastNumberIndex + 1, nextNumberIndex);
            i = nextNumberIndex;
        }
    }

    return stold(tokens[begin]);
}

int main() {

    while (true) {
        std::string input;
        std::cout << "> ";
        std::cin >> input;

        if (input == "exit") {
            break;
        }

        if (!isValidInput(input, validCharacters)) {
            std::cout << "Invalid input\n";
            continue;
        }

        std::cout << solve(tokenise(input)) << std::endl;
    }

    return 0;
}
