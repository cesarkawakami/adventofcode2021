#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::string> read_input() {
    std::string s;
    std::vector<std::string> rv;
    while (std::cin >> s) {
        rv.push_back(s);
    }
    return rv;
}

bool is_opening(char c) {
    return (c == '(' || c == '[' || c == '{' || c == '<');
}

int closing_score(char c) {
    if (c == ')') {
        return 3;
    }
    if (c == ']') {
        return 57;
    }
    if (c == '}') {
        return 1197;
    }
    if (c == '>') {
        return 25137;
    }
    throw std::logic_error{""};
}

char equivalent_opener(char c) {
    if (c == ')') {
        return '(';
    }
    if (c == ']') {
        return '[';
    }
    if (c == '}') {
        return '{';
    }
    if (c == '>') {
        return '<';
    }
    throw std::logic_error{""};
}

int solve_part_1(const std::vector<std::string> &input) {
    int total_score = 0;
    std::vector<char> stack;
    for (const auto &line : input) {
        stack.clear();
        for (char c : line) {
            if (is_opening(c)) {
                stack.push_back(c);
            } else {
                if (stack.empty() || *stack.rbegin() != equivalent_opener(c)) {
                    // std::cout << "adding " << closing_score(c) << "\n";
                    total_score += closing_score(c);
                    break;
                } else {
                    stack.pop_back();
                }
            }
        }
    }
    return total_score;
}

int64_t solve_part_2(const std::vector<std::string> &input) {
    std::vector<int64_t> scores;
    std::vector<char> stack;
    for (const auto &line : input) {
        stack.clear();
        bool valid = true;
        for (char c : line) {
            if (is_opening(c)) {
                stack.push_back(c);
            } else {
                if (stack.empty() || *stack.rbegin() != equivalent_opener(c)) {
                    valid = false;
                    break;
                } else {
                    stack.pop_back();
                }
            }
        }
        if (!valid) {
            continue;
        }
        int64_t score = std::accumulate(stack.rbegin(), stack.rend(), 0ll, [](int64_t acc, char c) {
            auto char_score_fn = [](char c) {
                if (c == '(') {
                    return 1;
                } else if (c == '[') {
                    return 2;
                } else if (c == '{') {
                    return 3;
                } else if (c == '<') {
                    return 4;
                }
                throw std::logic_error{""};
            };
            return acc * 5 + char_score_fn(c);
        });
        scores.push_back(score);
    }
    std::sort(scores.begin(), scores.end());
    return scores[std::ssize(scores) / 2];
}

int main() {
    auto input{read_input()};
    int part_1_answer = solve_part_1(input);
    std::cout << "part 1 answer: " << part_1_answer << "\n";
    std::cout << "part 2 answer: " << solve_part_2(input) << "\n";
}
