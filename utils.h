#pragma once
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

namespace utils {

// struct Timer {
//     std::clock_t start;
//     Timer() : start(std::clock()) {}
//     ~Timer() {
//         std::clock_t end = std::clock();
//         double elapsed_us = 1000000. * (end - start) / CLOCKS_PER_SEC;
//         std::cout << "cpu elapsed:  " << elapsed_us << " us\n";
//     }
// };

template <char... WhiteSpaceChars>
struct AoCCType : std::ctype<char> {
    static const mask *make_table() {
        static mask table[table_size]{};
        std::copy(classic_table(), classic_table() + table_size, table);
        std::for_each(table, table + table_size, [](mask &m) { m &= ~space; });
        for (char c : {WhiteSpaceChars...}) {
            table[c] |= space;
        }
        return table;
    }
    AoCCType(std::size_t refs = 0) : ctype(make_table(), false, refs) {}
    static void imbue(std::istream &is) {
        static AoCCType aoc_ctype;
        is.imbue(std::locale(is.getloc(), &aoc_ctype));
    }
};

std::string read_file(const std::string &filename) {
    const int BUF_SIZE = 100000;
    static char buf[BUF_SIZE];
    std::ifstream ins(filename);
    ins.read(buf, BUF_SIZE - 1);
    int len = ins.gcount();
    buf[len] = 0;
    return std::string{buf};
}

struct Tester {
    std::vector<std::tuple<std::string, bool, std::string>> results;
    void test(const std::string &name, const std::string &expected, auto solver) {
        std::cout << ">==  STARTING " << name << "  ==<\n";
        std::string input = read_file(name);
        std::istringstream iss{input};
        std::ostringstream oss;
        solver(iss, oss);
        const std::string &output = oss.str();
        std::cout << output << "\n";
        if (output.find(expected) != std::string::npos) {
            std::cout << "RESULT (" << name << "): OK!\n";
            results.push_back({name, true, "OK!"});
        } else {
            std::ostringstream msg;
            msg << "FAIL (expected " << expected << ")";
            std::cout << "RESULT (" << name << "): " << msg.str() << "\n";
            results.push_back({name, false, msg.str()});
        }
    }
    ~Tester() {
        std::cout << "\n>=====  RESULT SUMMARY  =====<\n";
        bool all_ok = true;
        for (const auto &[name, ok, msg] : results) {
            std::cout << "  " << name << ": " << msg << "\n";
            if (!ok) {
                all_ok = false;
            }
        }
        std::cout << "\n";
        if (!all_ok) {
            std::cout << "FAILED A TEST!" << "\n";
            abort();
        }
    }
};

void bench(const std::string &name, const std::string &expected, int iterations, auto solver) {
    using clock = std::chrono::high_resolution_clock;

    std::cout << ">=====  BENCH  =====<\n";
    std::cout << "  Running " << iterations << " iterations of " << name << "...\n";
    std::string input{read_file(name)};
    std::vector<double> timings;
    timings.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        std::istringstream iss{input};
        std::ostringstream oss;
        auto start = clock::now();
        solver(iss, oss);
        auto end = clock::now();
        std::string output{oss.str()};
        if (output.find(expected) == std::string::npos) {
            throw std::logic_error{"wrong answer!"};
        }
        // std::cout << "DEBUG: " << start << " " << end << CLOCKS_PER_SEC << "\n";
        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000.);
        // timings.push_back(1000000. * (end - start) / CLOCKS_PER_SEC);
    }
    std::cout << "  Done.\n";
    std::sort(timings.begin(), timings.end());
    std::cout << "\n";
    std::cout << "  pMin: " << timings[0] << " us\n";
    std::cout << "  p5  : " << timings[std::ssize(timings) / 20] << " us\n";
    std::cout << "  p50 : " << timings[std::ssize(timings) / 2] << " us\n";
}

// void timed_solve(auto solver) {
//     const int BUF_SIZE = 100000;
//     static char buf[BUF_SIZE];
//     std::cin.read(buf, BUF_SIZE - 1);
//     int len = std::cin.gcount();
//     buf[len] = 0;
//     std::istringstream iss{buf};
//     std::ostringstream oss;
//     {
//         Timer timer;
//         solver(iss, oss);
//     }
//     std::cout << oss.str();
// }

} // namespace utils
