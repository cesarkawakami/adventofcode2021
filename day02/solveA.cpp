#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::string command;
    int value;
    int depth = 0, x = 0;
    while (std::cin >> command >> value) {
        if (command == "forward") {
            x += value;
        } else if (command == "up") {
            depth -= value;
        } else if (command == "down") {
            depth += value;
        }
    }
    std::cout << depth * x << '\n';
}
