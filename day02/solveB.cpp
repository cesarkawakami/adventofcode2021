#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::string command;
    int value;
    int aim = 0, depth = 0, x = 0;
    while (std::cin >> command >> value) {
        if (command == "forward") {
            x += value;
            depth += value * aim;
        } else if (command == "up") {
            aim -= value;
        } else if (command == "down") {
            aim += value;
        }
    }
    std::cout << depth * x << '\n';
}
