#pragma once
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

namespace utils {

const bool BENCH_DISPLAY_INITIAL_RUNS = false;

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
    void test(const std::string &display_prefix, const std::string &name,
              const std::string &expected, auto solver) {
        std::ostringstream display_name;
        display_name << display_prefix << ":" << name;
        std::cout << ">==  " << display_name.str() << "  ==<\n";
        std::string input = read_file(name);
        std::istringstream iss{input};
        std::ostringstream oss;
        solver(iss, oss);
        const std::string &output = oss.str();

        std::cout << "RESULT (" << display_name.str() << "): ";
        std::ostringstream msg;
        bool ok = output == expected;
        if (ok) {
            msg << "OK!   got " << output;
        } else {
            msg << "FAIL! got " << output << " expected " << expected;
        }
        std::cout << msg.str() << "\n";
        results.push_back({display_name.str(), ok, msg.str()});
    }
    ~Tester() {
        std::cout << "\n>=====  RESULT SUMMARY  =====<\n";
        bool all_ok = true;
        for (const auto &[name, ok, msg] : results) {
            std::cout << "  " << name << "  " << msg << "\n";
            if (!ok) {
                all_ok = false;
            }
        }
        std::cout << "\n";
        if (!all_ok) {
            std::cout << "FAILED A TEST!\n";
            abort();
        }
    }
};

void bench(const std::string &display_prefix, const std::string &name, const std::string &expected,
           int iterations, auto solver) {
    using clock = std::chrono::high_resolution_clock;

    std::ostringstream display_name;
    display_name << display_prefix << ":" << name;

    std::cout << ">=====  BENCH  =====<\n";
    std::cout << "  Running " << iterations << " iterations of " << display_name.str() << "...\n";
    std::string input{read_file(name)};
    std::vector<double> timings;
    timings.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        std::istringstream iss{input};
        std::ostringstream oss;
        auto start = clock::now();
        solver(iss, oss);
        auto end = clock::now();
        const std::string &output{oss.str()};
        if (output != expected) {
            throw std::logic_error{"wrong answer!"};
        }
        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000.);
    }
    std::cout << "  Done.\n";

    if (BENCH_DISPLAY_INITIAL_RUNS) {
        std::cout << "\n";
        for (int i = 0; i < 5; ++i) {
            if (i >= std::ssize(timings)) {
                break;
            }
            std::cout << "  run" << i + 1 << ": " << timings[i] << " us\n";
        }
    }

    std::sort(timings.begin(), timings.end());
    std::cout << "\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  pMin: " << std::setw(7) << timings[0] << " us\n";
    std::cout << "  p5  : " << std::setw(7) << timings[std::ssize(timings) / 20] << " us\n";
    std::cout << "  p50 : " << std::setw(7) << timings[std::ssize(timings) / 2] << " us\n";
    std::cout << "  p95 : " << std::setw(7) << timings[std::ssize(timings) - std::ssize(timings) / 20] << " us\n";
    std::cout << "  pMax: " << std::setw(7) << timings[std::ssize(timings) - 1] << " us\n";
}

} // namespace utils
