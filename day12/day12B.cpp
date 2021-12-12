#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
#include "utils.h"

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
    bool visited[MAX_VERTICES];
    void read() {
        std::string line;
        while (std::cin >> line) {
            auto dash_pos = line.find('-');
            std::string a_name = line.substr(0, dash_pos);
            std::string b_name = line.substr(dash_pos + 1);
            int a = mapper.index_for(a_name);
            int b = mapper.index_for(b_name);
            // std::cout << "DEBUG: " << a << " " << a_name << " " << b << " " << b_name << "\n";
            neighbors[a].push(b);
            neighbors[b].push(a);
        }
    }
    // std::vector<int> path;
    int go(int v, bool can_burn) {
        assert(!visited[v] || can_burn);
        bool old_visited = visited[v];
        if (visited[v]) {
            can_burn = false;
        }
        // path.push_back(v);
        if (v == mapper.end) {
            // std::cout << "DEBUG:";
            // for (int x : path) {
            //     std::cout << " " << mapper.name_for(x);
            // }
            // std::cout << "\n";
            // path.pop_back();
            return 1;
        }
        if (!mapper.is_upper[v]) {
            visited[v] = true;
        }
        int path_count = 0;
        for (int neighbor : neighbors[v]) {
            if (!visited[neighbor] || (can_burn && neighbor != mapper.start)) {
                int sub_paths = go(neighbor, can_burn);
                path_count += sub_paths;
            }
        }
        visited[v] = old_visited;
        // path.pop_back();
        return path_count;
    }
    int solve() {
        std::fill(visited, visited + MAX_VERTICES, false);
        return go(mapper.start, true);
    }
};

int main() {
    Solver solver{};
    solver.read();
    int part_2_answer;
    {
        CPUTimer timer;
        part_2_answer = solver.solve();
    }
    std::cout << "part 2 answer: " << part_2_answer << "\n";
}
