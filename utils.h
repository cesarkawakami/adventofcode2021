#pragma once
#include <ctime>
#include <iostream>
#include <locale>
#include <vector>
#include <sstream>

struct CPUTimer {
    std::clock_t start;
    CPUTimer() : start(std::clock()) {}
    ~CPUTimer() {
        std::clock_t end = std::clock();
        double elapsed_us = 1000000. * (end - start) / CLOCKS_PER_SEC;
        std::cout << "cpu elapsed:  " << elapsed_us << " us\n";
    }
};

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

void timed_solve(auto solver) {
    const int BUF_SIZE = 100000;
    static char buf[BUF_SIZE];
    std::cin.read(buf, BUF_SIZE - 1);
    int len = std::cin.gcount();
    buf[len] = 0;
    std::istringstream iss{buf};
    std::ostringstream oss;
    {
        CPUTimer timer;
        solver(iss, oss);
    }
    std::cout << oss.str();
}