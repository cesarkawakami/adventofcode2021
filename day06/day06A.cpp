#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <numeric>

using count_type = int64_t;

std::map<int, count_type> read_fish() {
    std::string line;
    std::getline(std::cin, line);
    line = std::regex_replace(line, std::regex{","}, " ");
    std::istringstream line_ss{line};
    std::map<int, count_type> fish;
    int x;
    while (line_ss >> x) {
        fish[x] += 1;
    }
    return fish;
}

std::map<int, count_type> next_day(const std::map<int, count_type> current_day) {
    std::map<int, count_type> next_day;
    for (auto [fish_type, fish_count] : current_day) {
        if (fish_type == 0) {
            next_day[6] += fish_count;
            next_day[8] += fish_count;
        } else {
            next_day[fish_type - 1] += fish_count;
        }
    }
    return next_day;
}

int main() {
    std::map<int, count_type> initial_fish{read_fish()};
    std::map<int, count_type> current_fish{initial_fish};
    for (int day_index = 0; day_index <= 256; ++day_index) {
        count_type total = std::accumulate(current_fish.begin(), current_fish.end(), count_type{0},
                                           [&](auto acc, auto pair) { return acc + pair.second; });
        std::cout << "day: " << day_index << " total: " << total << "\n";
        current_fish = next_day(current_fish);
    }
}
