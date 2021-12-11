#include <algorithm>
#include <iostream>
#include <locale>
#include <numeric>
#include <vector>

std::vector<std::vector<int8_t>> read_heightmap() {
    std::string line;
    std::vector<std::vector<int8_t>> rv;
    while (std::cin >> line) {
        std::vector<int8_t> row(std::size(line));
        std::transform(line.begin(), line.end(), row.begin(), [](char c) { return c - '0'; });
        rv.push_back(row);
    }
    return rv;
}

const std::initializer_list<std::tuple<int, int>> DIRS{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::vector<std::tuple<int, int>> find_lowest_points(const std::vector<std::vector<int8_t>> &heightmap) {
    auto get_height = [&](int r, int c) -> int8_t {
        if (r < 0 || r >= std::ssize(heightmap) || c < 0 || c >= std::ssize(heightmap[r])) {
            return 20;
        }
        return heightmap[r][c];
    };
    std::vector<std::tuple<int, int>> lowest_points;
    for (int r = 0; r < std::ssize(heightmap); ++r) {
        for (int c = 0; c < std::ssize(heightmap[r]); ++c) {
            bool is_still_lowest = true;
            for (auto [dr, dc] : DIRS) {
                if (get_height(r, c) >= get_height(r + dr, c + dc)) {
                    is_still_lowest = false;
                    break;
                }
            }
            if (is_still_lowest) {
                lowest_points.push_back({r, c});
            }
        }
    }
    return lowest_points;
}

struct Solver {
    std::vector<std::vector<int8_t>> heightmap;
    std::vector<std::vector<int8_t>> visited;

    int fill(int start_r, int start_c) {
        std::vector<std::tuple<int, int>> stack{{start_r, start_c}};
        int size = 0;
        while (!stack.empty()) {
            auto [r, c] = stack.back();
            stack.pop_back();
            if (visited[r][c]) {
                continue;
            }
            visited[r][c] = true;
            ++size;
            for (auto [dr, dc] : DIRS) {
                if (0 <= r + dr && r + dr < std::ssize(heightmap) &&
                    0 <= c + dc && c + dc < std::ssize(heightmap[r]) &&
                    !visited[r + dr][c + dc] && heightmap[r + dr][c + dc] != 9) {
                    stack.push_back({r + dr, c + dc});
                }
            }
        }
        return size;
    }

    std::vector<int> find_basins() {
        visited.clear();
        std::transform(heightmap.begin(), heightmap.end(), std::back_inserter(visited),
                       [](const auto &row) { return std::vector<int8_t>(std::size(row), false); });
        std::vector<int> basin_sizes;
        for (int r = 0; r < std::ssize(heightmap); ++r) {
            for (int c = 0; c < std::ssize(heightmap[r]); ++c) {
                if (heightmap[r][c] != 9 && !visited[r][c]) {
                    basin_sizes.push_back(fill(r, c));
                }
            }
        }
        return basin_sizes;
    }
};

int main() {
    auto heightmap{read_heightmap()};
    auto lowest_points{find_lowest_points(heightmap)};
    int lowest_points_risk = std::accumulate(lowest_points.begin(), lowest_points.end(), 0,
                                             [&](int acc, auto p) {
                                                 const auto &[r, c] = p;
                                                 return acc + 1 + heightmap[r][c];
                                             });
    std::cout << "part 1 answer: " << lowest_points_risk << "\n";

    Solver solver{heightmap};
    std::vector<int> basin_sizes = solver.find_basins();
    std::sort(basin_sizes.begin(), basin_sizes.end());
    int part_2_answer = std::accumulate(basin_sizes.rbegin(), basin_sizes.rbegin() + 3, 1, [](int acc, int size) { return acc * size; });
    std::cout << "part 2 answer: " << part_2_answer << "\n";
}
