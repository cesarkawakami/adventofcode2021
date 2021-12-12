#include "utils.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <numeric>
#include <vector>

const int N = 10;

const std::initializer_list<std::tuple<int8_t, int8_t>> DIRS{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

bool valid(int8_t r, int8_t c) {
    return 0 <= r && r < N && 0 <= c && c < N;
}

template <typename T, int Limit>
struct InlineStack {
    int size = 0;
    T data[Limit]{};

    void push(const T &value) {
        data[size++] = value;
    }

    T pop() {
        return data[--size];
    }

    bool empty() const {
        return size == 0;
    }
};

struct Octopi {
    int8_t data[N][N];

    void read(std::istream &is) {
        std::string s;
        for (int r = 0; r < N; ++r) {
            is >> s;
            for (int c = 0; c < N; ++c) {
                data[r][c] = s[c] - '0';
            }
        }
    }

    int step() {
        std::for_each(data, data + N, [](auto &row) {
            std::for_each(row, row + N, [](int8_t &val) {
                val += 1;
            });
        });

        bool flashed[N][N]{};
        InlineStack<std::tuple<int8_t, int8_t>, 200> stack;
        auto check_flash{[&](int8_t r, int8_t c) {
            if (valid(r, c) && data[r][c] > 9 && !flashed[r][c]) {
                flashed[r][c] = true;
                stack.push({r, c});
            }
        }};

        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                check_flash(r, c);
            }
        }
        int flash_count = 0;
        while (!stack.empty()) {
            auto [r, c] = stack.pop();
            ++flash_count;
            for (auto [dr, dc] : DIRS) {
                if (valid(r + dr, c + dc)) {
                    data[r + dr][c + dc] += 1;
                    check_flash(r + dr, c + dc);
                }
            }
        }
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (flashed[r][c]) {
                    data[r][c] = 0;
                }
            }
        }
        return flash_count;
    }
};

auto solve_part_2{[](std::istream &is, std::ostream &os) {
    Octopi octopi{};
    octopi.read(is);

    int part_2_answer;
    for (int step_number = 1;; ++step_number) {
        int flash_count = octopi.step();
        if (flash_count == N * N) {
            part_2_answer = step_number;
            break;
        }
    }
    os << part_2_answer;
}};

int main() {
    {
        utils::Tester tester;
        tester.test("part2", "in1", "195", solve_part_2);
        tester.test("part2", "in2", "212", solve_part_2);
    }

    utils::bench("part2", "in2", "212", 1000, solve_part_2);
}
