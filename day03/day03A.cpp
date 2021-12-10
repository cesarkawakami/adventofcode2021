#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

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
    std::string gamma, epsilon;
    gamma.resize(numbers[0].size());
    epsilon.resize(numbers[0].size());
    for (int i = 0; i < std::ssize(gamma); ++i) {
        int one_count = std::count_if(std::begin(numbers), std::end(numbers), [=](const auto &n) {
            return n[i] == '1';
        });
        if (one_count >= std::ssize(numbers) / 2) {
            gamma[i] = '1';
            epsilon[i] = '0';
        } else {
            gamma[i] = '0';
            epsilon[i] = '1';
        }
    }
    std::cout << "gamma: " << gamma << " " << bin_to_int(gamma) << '\n';
    std::cout << "epsilon: " << epsilon << " " << bin_to_int(epsilon) << '\n';
    std::cout << "answer: " << bin_to_int(gamma) * bin_to_int(epsilon) << '\n';
}
