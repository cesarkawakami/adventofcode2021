#include "utils.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

const int MAX_VERTICES = 20;
const int MAX_NEIGHBORS = 10;

template <typename T, int Limit>
struct InlineVec {
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
    T *begin() {
        return &data[0];
    }
    T *end() {
        return &data[size];
    }
};

struct Mapper {
    int current_index = 0;
    std::map<std::string, int> name_to_index;
    std::string index_to_name[MAX_VERTICES]{};
    bool is_upper[MAX_VERTICES]{};
    int start = -1;
    int end = -1;

    int index_for(const std::string &name) {
        if (!name_to_index.contains(name)) {
            name_to_index[name] = current_index;
            index_to_name[current_index] = name;
            if (name == "start") {
                start = current_index;
            }
            if (name == "end") {
                end = current_index;
            }
            if (std::isupper(name[0])) {
                is_upper[current_index] = true;
            }
            return current_index++;
        }
        return name_to_index[name];
    }
    std::string name_for(int index) const {
        return index_to_name[index];
    }
};

struct Solver {
    Mapper mapper;
    InlineVec<int, MAX_NEIGHBORS> neighbors[MAX_VERTICES];
    int dp[2][11][1 << 11];
    void read(std::istream &is) {
        std::string line;
        while (is >> line) {
            auto dash_pos = line.find('-');
            std::string a_name = line.substr(0, dash_pos);
            std::string b_name = line.substr(dash_pos + 1);
            int a = mapper.index_for(a_name);
            int b = mapper.index_for(b_name);
            neighbors[a].push(b);
            neighbors[b].push(a);
        }
    }
    bool has_visited(int vmask, int v) const {
        return (vmask & (1 << v)) != 0;
    }
    int with_visit(int vmask, int v, bool flag) const {
        return (vmask & (~(1 << v))) | (int{flag} << v);
    }
    int go(const int vmask, const int v, const bool can_burn) {
        assert(!has_visited(vmask, v) || can_burn);
        if (dp[can_burn][v][vmask] != -1) {
            return dp[can_burn][v][vmask];
        }
        int new_vmask = vmask;
        bool new_can_burn = can_burn;
        if (has_visited(vmask, v)) {
            new_can_burn = false;
        }
        if (v == mapper.end) {
            return 1;
        }
        if (!mapper.is_upper[v]) {
            new_vmask = with_visit(vmask, v, true);
        }
        int path_count = 0;
        for (int neighbor : neighbors[v]) {
            if (!has_visited(vmask, neighbor) || (new_can_burn && neighbor != mapper.start)) {
                int sub_paths = go(new_vmask, neighbor, new_can_burn);
                path_count += sub_paths;
            }
        }
        dp[can_burn][v][vmask] = path_count;
        return path_count;
    }
    int solve() {
        std::memset(dp, -1, sizeof(dp));
        return go(0, mapper.start, true);
    }
};

auto solve_part_2 = [](std::istream &is, std::ostream &os) {
    Solver solver{};
    solver.read(is);
    int part_2_answer = solver.solve();
    os << part_2_answer;
};

int main() {
    {
        utils::Tester tester;
        tester.test("part2", "in1", "36", solve_part_2);
        tester.test("part2", "in2", "103", solve_part_2);
        tester.test("part2", "in3", "3509", solve_part_2);
        tester.test("part2", "in4", "117095", solve_part_2);
    }

    utils::bench("part2", "in4", "117095", 10000, solve_part_2);
}
