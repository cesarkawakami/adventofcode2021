#include "utils.h"
#include <cassert>
#include <iterator>
#include <set>

enum struct AxisDir { X,
                      Y };

auto flip = [](const std::tuple<int, int> &t) -> std::tuple<int, int> {
    auto [x, y] = t;
    return {y, x};
};

struct Solver {
    std::set<std::tuple<int, int>> dots;
    std::vector<std::tuple<AxisDir, int>> folds;

    void read(std::istream &is) {
        utils::WhitespaceCtype<' ', '\n', ',', '='>::imbue(is);
        int x, y;
        while (is >> x >> y) {
            dots.insert({x, y});
        }
        is.clear();
        std::string fold, along, axis;
        int coord_value;
        while (is >> fold >> along >> axis >> coord_value) {
            assert(fold == "fold" && along == "along");
            AxisDir axis_dir = (axis == "x") ? AxisDir::X : AxisDir::Y;
            folds.push_back({axis_dir, coord_value});
        }
    }

    void flip_all() {
        std::vector<std::tuple<int, int>> tmp(dots.begin(), dots.end());
        dots.clear();
        std::transform(tmp.begin(), tmp.end(), std::inserter(dots, dots.end()), flip);
    }

    void fold(AxisDir axis_dir, int coord) {
        if (axis_dir == AxisDir::Y) {
            flip_all();
        }

        std::vector<std::tuple<int, int>> tmp;
        for (auto it = dots.begin(); it != dots.end();) {
            auto [x, y] = *it;
            if (x >= coord) {
                if (x > coord) {
                    tmp.push_back({2 * coord - x, y});
                }
                it = dots.erase(it);
            } else {
                ++it;
            }
        }
        std::copy(tmp.begin(), tmp.end(), std::inserter(dots, dots.end()));

        if (axis_dir == AxisDir::Y) {
            flip_all();
        }
    }
};

auto solve1 = [](std::istream &is, std::ostream &os) -> void {
    Solver solver;
    solver.read(is);
    // for (const auto &[x, y] : solver.dots) {
    //     std::cout << "DEBUG: dot at " << x << " " << y << "\n";
    // }
    // for (const auto &[axis_dir, coord] : solver.folds) {
    //     std::cout << "DEBUG: fold at " << ((axis_dir == AxisDir::X) ? "X" : "Y") << " " << coord << "\n";
    // }
    const auto &[axis_dir, coord] = solver.folds[0];
    solver.fold(axis_dir, coord);
    os << std::size(solver.dots);
};

auto solve2 = [](std::istringstream &is, std::ostringstream &os) -> void {
    Solver solver;
    solver.read(is);
    for (const auto &[axis_dir, coord] : solver.folds) {
        solver.fold(axis_dir, coord);
    }
    int max_x = -1, max_y = -1;
    for (const auto &[x, y] : solver.dots) {
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }
    for (int y = 0; y <= max_y; ++y) {
        for (int x = 0; x <= max_x; ++x) {
            if (solver.dots.contains({x, y})) {
                os << "#";
            } else {
                os << ".";
            }
        }
        os << "\n";
    }
    // std::cout << os.str();
};

int main() {
    {
        utils::Tester tester;
        tester.test("part1", "in1", "17", solve1);
        tester.test("part1", "in2", "737", solve1);
        tester.test("part2", "in1",
                    "#####\n"
                    "#...#\n"
                    "#...#\n"
                    "#...#\n"
                    "#####\n",
                    solve2);
        tester.test("part2", "in2",
                    "####.#..#...##.#..#..##..####.#..#.###.\n"
                    "...#.#..#....#.#..#.#..#.#....#..#.#..#\n"
                    "..#..#..#....#.#..#.#..#.###..####.#..#\n"
                    ".#...#..#....#.#..#.####.#....#..#.###.\n"
                    "#....#..#.#..#.#..#.#..#.#....#..#.#...\n"
                    "####..##...##...##..#..#.#....#..#.#...\n",
                    solve2);
    }

    utils::bench("part2", "in2",
                 "####.#..#...##.#..#..##..####.#..#.###.\n"
                 "...#.#..#....#.#..#.#..#.#....#..#.#..#\n"
                 "..#..#..#....#.#..#.#..#.###..####.#..#\n"
                 ".#...#..#....#.#..#.####.#....#..#.###.\n"
                 "#....#..#.#..#.#..#.#..#.#....#..#.#...\n"
                 "####..##...##...##..#..#.#....#..#.#...\n",
                 100,
                 solve2);
}
