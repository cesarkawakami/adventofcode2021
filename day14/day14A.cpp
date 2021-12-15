#include "utils.h"
#include <cassert>
#include <iterator>
#include <map>
#include <set>

struct Solver {
    std::string initial;
    std::map<std::tuple<char, char>, char> rules;

    void read(std::istream &is) {
        is >> initial;
        std::string a, b, c;
        while (is >> a >> b >> c) {
            assert(b == "->");
            rules[{a[0], a[1]}] = c[0];
        }
    }

    std::string iterate(const std::string &s) {
        std::string result{s.begin(), s.begin() + 1};
        for (int i = 1; i < std::ssize(s); ++i) {
            std::tuple<char, char> key{s[i - 1], s[i]};
            result.append({rules.at({s[i - 1], s[i]}), s[i]});
        }
        return result;
    }
};

auto solve1 = [](std::istream &is, std::ostream &os) {
    Solver solver;
    solver.read(is);
    std::string s{solver.initial};
    for (int i = 0; i < 10; ++i) {
        s = solver.iterate(s);
    }

    std::map<char, int> freqs;
    std::for_each(s.begin(), s.end(), [&](char c) { ++freqs[c]; });
    std::vector<std::tuple<int, char>> freqs2;
    std::transform(freqs.begin(), freqs.end(), std::back_inserter(freqs2), [](const auto &tuple) -> std::tuple<int, char> {
        const auto &[c, freq] = tuple;
        return {freq, c};
    });
    const auto &[max_freq, max_char] = *std::max_element(freqs2.begin(), freqs2.end());
    const auto &[min_freq, min_char] = *std::min_element(freqs2.begin(), freqs2.end());
    const int answer = max_freq - min_freq;

    os << answer;
};

int main() {
    {
        utils::Tester tester;
        tester.test("part1", "in1", "1588", solve1);
        tester.test("part1", "in2", "3555", solve1);
        // tester.test("part2", "in1", "", solve2);
        // tester.test("part2", "in2", "", solve2);
    }

    // utils::bench("part2", "in2", "", 100, solve2);
}
