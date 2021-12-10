#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>

template <typename Comparator>
std::string solve_find(std::map<std::string, int> &counts,
                       int len,
                       const Comparator &comparator,
                       char if_equal) {
    std::string prefix;
    for (int i = 0; i < len; ++i) {
        int zero_cnt = counts[prefix + '0'];
        int one_cnt = counts[prefix + '1'];
        if (one_cnt + zero_cnt == 1) {
            prefix += one_cnt ? '1' : '0';
        } else if (one_cnt == zero_cnt) {
            prefix += if_equal;
        } else {
            prefix += comparator(one_cnt, zero_cnt) ? '1' : '0';
        }
    }
    return prefix;
}

int bin_to_int(const std::string &s) {
    int rv = 0;
    std::for_each(std::begin(s), std::end(s), [&](auto c) {
        rv = (rv << 1) | (c - '0');
    });
    return rv;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::vector<std::string> numbers;
    std::copy(std::istream_iterator<std::string>(std::cin),
              std::istream_iterator<std::string>(),
              std::back_inserter(numbers));

    std::map<std::string, int> counts;
    std::for_each(std::begin(numbers), std::end(numbers), [&](const auto &number) {
        std::string prefix;
        std::for_each(std::begin(number), std::end(number), [&](const auto c) {
            prefix += c;
            counts[prefix] += 1;
        });
    });

    std::string o2_gen = solve_find(counts, std::ssize(numbers[0]), std::greater<int>(), '1');
    std::string co2_scr = solve_find(counts, std::ssize(numbers[0]), std::less<int>(), '0');
    int o2_gen_int = bin_to_int(o2_gen);
    int co2_scr_int = bin_to_int(co2_scr);
    std::cout << "o2 gen: " << o2_gen << " " << o2_gen_int << '\n';
    std::cout << "co2 scr: " << co2_scr << " " << co2_scr_int << '\n';
    std::cout << "answer: " << o2_gen_int * co2_scr_int << '\n';
}
