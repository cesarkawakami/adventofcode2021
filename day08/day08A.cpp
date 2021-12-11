#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <vector>

const std::vector<std::string> original_reference{"abcefg", "cf", "acdeg", "acdfg", "bcdf", "abdfg", "abdefg", "acf", "abcdefg", "abcdfg"};

void adjust_patterns(std::vector<std::string> &patterns) {
    std::for_each(patterns.begin(), patterns.end(), [](auto &s) { std::sort(s.begin(), s.end()); });
    std::sort(patterns.begin(), patterns.end());
}

std::vector<std::string> make_reference() {
    std::vector<std::string> reference{original_reference};
    adjust_patterns(reference);
    return reference;
}

const std::vector<std::string> reference{make_reference()};

std::string to_string(const std::vector<std::string> &v) {
    std::ostringstream ss;
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(ss, " "));
    return ss.str();
}

std::string to_string(const std::tuple<std::vector<std::string>, std::vector<std::string>> &v) {
    std::ostringstream ss;
    const auto &[a, b] = v;
    ss << to_string(a) << " | " << to_string(b);
    return ss.str();
}

bool is_consistent(const std::vector<std::string> &patterns, const std::string &perm) {
    std::vector<std::string> permuted_patterns(std::size(patterns));
    std::transform(
        patterns.begin(), patterns.end(), permuted_patterns.begin(),
        [&](const auto &unpermuted_pattern) {
            std::string permuted_pattern(std::size(unpermuted_pattern), ' ');
            std::transform(unpermuted_pattern.begin(), unpermuted_pattern.end(), permuted_pattern.begin(),
                           [&](char c) { return perm[c - 'a']; });
            return permuted_pattern;
        });
    adjust_patterns(permuted_patterns);
    // std::cout << "DEBUG: perm=" << perm << " pats:" << to_string(permuted_patterns) << "\n";
    return std::equal(reference.begin(), reference.end(), permuted_patterns.begin());
}

std::vector<std::string> solve_permutations(const std::vector<std::string> &patterns) {
    std::string perm{"abcdefg"};
    std::vector<std::string> valid_perms;
    do {
        if (is_consistent(patterns, perm)) {
            valid_perms.push_back(perm);
        }
    } while (std::next_permutation(perm.begin(), perm.end()));
    return valid_perms;
}

int decode(const std::string &perm, std::string encoded) {
    std::string decoded(std::size(encoded), ' ');
    std::transform(encoded.begin(), encoded.end(), decoded.begin(), [&](char c) { return perm[c - 'a']; });
    std::sort(decoded.begin(), decoded.end());
    if (auto it = std::find(original_reference.begin(), original_reference.end(), decoded); it != original_reference.end()) {
        return it - original_reference.begin();
    }
    throw std::logic_error{"unable to decode!"};
}

std::optional<std::tuple<std::vector<std::string>, std::vector<std::string>>> read_input_line() {
    std::tuple<std::vector<std::string>, std::vector<std::string>> rv{{}, {}};
    auto &[patterns, encoded_digits] = rv;
    for (int i = 0; i < 10; ++i) {
        std::string pattern;
        if (std::cin >> pattern) {
            patterns.push_back(pattern);
        } else {
            return std::nullopt;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '|');
    for (int i = 0; i < 4; ++i) {
        std::string encoded_digit;
        if (std::cin >> encoded_digit) {
            encoded_digits.push_back(encoded_digit);
        } else {
            return std::nullopt;
        }
    }
    return rv;
}

std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>>> read_input() {
    std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>>> rv;
    while (auto val = read_input_line()) {
        rv.push_back(*val);
    }
    return rv;
}

int main() {
    assert(std::all_of(original_reference.begin(), original_reference.end(),
                       [](const auto &s) { return std::is_sorted(s.begin(), s.end()); }));
    assert(std::is_sorted(reference.begin(), reference.end()));

    const auto input = read_input();

    std::vector<std::vector<int>> all_solves;
    for (const auto &[patterns, encoded_digits] : input) {
        std::cout << "solving: " << to_string(std::tuple{patterns, encoded_digits}) << "\n";
        std::vector<std::string> valid_perms = solve_permutations(patterns);
        std::cout << "# valid perms: " << std::size(valid_perms) << "\n";
        std::copy_n(valid_perms.begin(), std::min(5L, std::ssize(valid_perms)), std::ostream_iterator<std::string>(std::cout, "\n"));

        if (std::ssize(valid_perms) != 1) {
            throw std::logic_error{"uh oh"};
        }

        const auto &perm = valid_perms[0];

        std::vector<int> solved_digits;
        std::transform(encoded_digits.begin(), encoded_digits.end(), std::back_inserter(solved_digits), [&](const auto &digit) {
            int x = decode(perm, digit);
            // std::cout << "DEBUG: " << digit << " " << x << "\n";
            return x;
        });
        std::cout << "solved: ";
        std::copy(solved_digits.begin(), solved_digits.end(), std::ostream_iterator<int>(std::cout, ""));
        std::cout << "\n";
        all_solves.push_back(solved_digits);
    }

    int part_1_count = std::accumulate(all_solves.begin(), all_solves.end(), 0, [&](int acc, const auto &digits) {
        return acc + std::accumulate(digits.begin(), digits.end(), 0, [&](int acc, int digit) {
            if (digit == 1 || digit == 4 || digit == 7 || digit == 8) {
                return acc + 1;
            } else {
                return acc;
            }
        });
    });
    std::cout << "part 1 answer: " << part_1_count << "\n";

    int part_2_answer = std::accumulate(all_solves.begin(), all_solves.end(), 0, [&](int acc, const auto &digits) {
        return acc + std::accumulate(digits.begin(), digits.end(), 0, [&](int acc, int digit) {
            return 10 * acc + digit;
        });
    });
    std::cout << "part 2 answer: " << part_2_answer << "\n";
}
