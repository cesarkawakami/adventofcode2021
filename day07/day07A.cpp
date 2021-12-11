#include <iostream>
#include <locale>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

struct AoCCType : std::ctype<char> {
    static const mask *make_table() {
        // make a copy of the "C" locale table
        static std::vector<mask> v(classic_table(), classic_table() + table_size);
        v[','] |= space;
        return &v[0];
    }
    AoCCType(std::size_t refs = 0) : ctype(make_table(), false, refs) {}
    static void imbue() {
        static AoCCType aoc_ctype;
        std::cin.imbue(std::locale(std::cin.getloc(), &aoc_ctype));
    }
};

int quad_fuel(int a, int b) {
    int delta = std::abs(b - a);
    return delta * (delta + 1) / 2;
}

int quad_total(const std::vector<int> &positions, int center) {
    return std::accumulate(positions.begin(), positions.end(), 0,
                           [&](int acc, int pos) { return acc + quad_fuel(center, pos); });
}

int run_search(const std::vector<int> &positions) {
    int left = *positions.begin();
    int right = *positions.rbegin();
    while (right - left >= 10) {
        int lmid = left + (right - left) / 3;
        int rmid = left + (right - left) / 3 * 2;
        int lmid_cost = quad_total(positions, lmid);
        int rmid_cost = quad_total(positions, rmid);
        if (lmid_cost < rmid_cost) {
            right = rmid;
        } else if (lmid_cost > rmid_cost) {
            left = lmid;
        } else {
            left = lmid;
            right = rmid;
        }
    }
    int min_fuel = std::numeric_limits<int>::max();
    for (int p = left; p <= right; ++p) {
        int fuel = quad_total(positions, p);
        if (fuel < min_fuel) {
            min_fuel = fuel;
        }
    }
    return min_fuel;
}

int main() {
    AoCCType::imbue();
    std::vector<int> positions;
    std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(),
              std::back_inserter(positions));
    std::sort(positions.begin(), positions.end());
    int center = positions[std::ssize(positions) / 2];
    int fuel = std::accumulate(positions.begin(), positions.end(), 0,
                               [&](int acc, int pos) { return acc + std::abs(pos - center); });
    std::cout << "part 1 answer: " << fuel << "\n";

    int quadratic_fuel_cost = run_search(positions);
    std::cout << "part 2 answer: " << quadratic_fuel_cost << "\n";
}
