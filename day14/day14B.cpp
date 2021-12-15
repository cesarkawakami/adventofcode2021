#include "utils.h"
#include <cassert>
#include <iterator>
#include <map>
#include <set>

const int MAXC = 26;

int toi(const std::tuple<int, int> &p) {
    const auto &[a, b] = p;
    return a * MAXC + b;
}

template <typename T>
struct CharPairMap {
    T freqs[MAXC * MAXC]{};

    T &operator[](const std::tuple<int, int> &p) {
        return freqs[toi(p)];
    }

    const T &operator[](const std::tuple<int, int> &p) const {
        return freqs[toi(p)];
    }
};

struct Solver {
    int leftmost, rightmost;
    CharPairMap<int64_t> initial;
    CharPairMap<int> rules;

    void clear() {
        initial = {};
        for (int i = 0; i < MAXC; ++i) {
            for (int j = 0; j < MAXC; ++j) {
                rules[{i, j}] = -1;
            }
        }
    }

    void read(std::istream &is) {
        std::string s;
        is >> s;
        leftmost = *s.begin() - 'A';
        rightmost = *s.rbegin() - 'A';
        for (int i = 1; i < std::ssize(s); ++i) {
            ++initial[{s[i - 1] - 'A', s[i] - 'A'}];
        }
        char a, b, c, d, e;
        while (is >> a >> b >> c >> d >> e) {
            assert(c == '-' && d == '>');
            rules[{a - 'A', b - 'A'}] = e - 'A';
        }
    }

    CharPairMap<int64_t> iterate1(const CharPairMap<int64_t> &freqs) const {
        CharPairMap<int64_t> result;
        for (int i = 0; i < MAXC; ++i) {
            for (int j = 0; j < MAXC; ++j) {
                int middle = rules[{i, j}];
                if (middle != -1) {
                    result[{i, middle}] += freqs[{i, j}];
                    result[{middle, j}] += freqs[{i, j}];
                }
            }
        }
        return result;
    }
};

Solver solver;

auto solve = [](std::istream &is, std::ostream &os, int iterations) {
    solver.clear();
    solver.read(is);
    CharPairMap<int64_t> freqs3{solver.initial};
    for (int i = 0; i < iterations; ++i) {
        freqs3 = solver.iterate1(freqs3);
    }

    int64_t freqs[MAXC]{};
    for (int i = 0; i < MAXC; ++i) {
        for (int j = 0; j < MAXC; ++j) {
            freqs[i] += freqs3[{i, j}];
            freqs[j] += freqs3[{i, j}];
        }
    }
    ++freqs[solver.leftmost];
    ++freqs[solver.rightmost];
    for (int i = 0; i < MAXC; ++i) {
        freqs[i] /= 2;
    }

    std::vector<std::tuple<int64_t, int>> freqs2;
    for (int i = 0; i < MAXC; ++i) {
        if (freqs[i] != 0) {
            freqs2.push_back({freqs[i], i});
        }
    }
    const auto &[max_freq, max_char] = *std::max_element(freqs2.begin(), freqs2.end());
    const auto &[min_freq, min_char] = *std::min_element(freqs2.begin(), freqs2.end());
    const int64_t answer = max_freq - min_freq;

    os << answer;
};

auto solve1 = [](std::istream &is, std::ostream &os) {
    return solve(is, os, 10);
};

auto solve2 = [](std::istream &is, std::ostream &os) {
    return solve(is, os, 40);
};

int main() {
    {
        utils::Tester tester;
        tester.test("part1", "in1", "1588", solve1);
        tester.test("part1", "in2", "3555", solve1);
        tester.test("part2", "in1", "2188189693529", solve2);
        tester.test("part2", "in2", "4439442043739", solve2);
    }

    utils::bench("part2", "in2", "4439442043739", 50000, solve2);
}
