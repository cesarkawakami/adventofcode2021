#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iterator>
#include <numeric>
#include <set>
#include <string>

enum struct AxisDir { X,
                      Y };

const int MAXX = 42;
const int MAXY = 8;

struct Matrix {
    int maxx, maxy;
    char data[MAXY][MAXX];
};

struct Reader {
    const char *p;
    const char *p_end;
    std::vector<std::tuple<int, int>> &dots;
    std::vector<int> &x_folds;
    std::vector<int> &y_folds;

    int read_int() {
        int v = 0;
        while (true) {
            char c = *p++;
            if ('0' <= c && c <= '9') {
                v = 10 * v + (c - '0');
            } else {
                break;
            }
        }
        return v;
    }

    std::tuple<bool, int, int> read_dot() {
        if (*p == '\n') {
            ++p;
            return {false, -1, -1};
        }
        int x = read_int();
        int y = read_int();
        return {true, x, y};
    }

    std::tuple<bool, char, int> read_fold() {
        if (p == p_end || p + 1 == p_end) {
            return {false, -1, -1};
        }
        while (*p != '=') {
            ++p;
        }
        char axis = *(p - 1);
        ++p;
        int coord = read_int();
        return {true, axis, coord};
    }

    void read() {
        while (true) {
            const auto &[valid, x, y] = read_dot();
            if (valid) {
                dots.push_back({x, y});
            } else {
                break;
            }
        }
        while (true) {
            const auto &[valid, axis, coord] = read_fold();
            if (valid) {
                if (axis == 'x') {
                    x_folds.push_back(coord);
                } else {
                    y_folds.push_back(coord);
                }
            } else {
                break;
            }
        }
    }
};

struct Solver {
    std::vector<std::tuple<int, int>> dots;
    std::vector<int> x_folds;
    std::vector<int> y_folds;

    void read(std::istringstream &is) {
        std::string s{is.str()};
        Reader reader{s.c_str(), s.c_str() + s.length(), dots, x_folds, y_folds};
        reader.read();
    }

    Matrix fold() {
        auto fold_single = [](int x, int fold) {
            if (x > fold) {
                return 2 * fold - x;
            } else {
                return x;
            }
        };

        int maxx = *std::min_element(x_folds.begin(), x_folds.end()) - 1;
        int maxy = *std::min_element(y_folds.begin(), y_folds.end()) - 1;

        Matrix matrix{maxx, maxy, {}};
        for (int y = 0; y <= maxy; ++y) {
            for (int x = 0; x <= maxx; ++x) {
                matrix.data[y][x] = '.';
            }
        }
        int prevy = -1, foldedy;
        std::sort(dots.begin(), dots.end(), [](const auto &a, const auto &b) {
            const auto &[ax, ay] = a;
            const auto &[bx, by] = b;
            if (ay != by) {
                return ay < by;
            } else {
                return ax < bx;
            }
        });
        for (const auto &[ox, oy] : dots) {
            if (oy != prevy) {
                foldedy = std::accumulate(y_folds.begin(), y_folds.end(), oy, fold_single);
                prevy = oy;
            }
            int foldedx = std::accumulate(x_folds.begin(), x_folds.end(), ox, fold_single);
            matrix.data[foldedy][foldedx] = '#';
        }
        return matrix;
    }
};

auto solve2 = [](std::istringstream &is, std::ostringstream &os) -> void {
    Solver solver;
    solver.read(is);
    Matrix matrix{solver.fold()};
    for (int y = 0; y <= matrix.maxy; ++y) {
        os << matrix.data[y] << "\n";
    }
    // std::cout << os.str();
};

int main() {
    {
        utils::Tester tester;
        tester.test("part2", "in1",
                    "#####\n"
                    "#...#\n"
                    "#...#\n"
                    "#...#\n"
                    "#####\n"
                    ".....\n"
                    ".....\n",
                    solve2);
        tester.test("part2", "in2",
                    "####.#..#...##.#..#..##..####.#..#.###..\n"
                    "...#.#..#....#.#..#.#..#.#....#..#.#..#.\n"
                    "..#..#..#....#.#..#.#..#.###..####.#..#.\n"
                    ".#...#..#....#.#..#.####.#....#..#.###..\n"
                    "#....#..#.#..#.#..#.#..#.#....#..#.#....\n"
                    "####..##...##...##..#..#.#....#..#.#....\n",
                    solve2);
    }

    utils::bench("part2", "in2",
                 "####.#..#...##.#..#..##..####.#..#.###..\n"
                 "...#.#..#....#.#..#.#..#.#....#..#.#..#.\n"
                 "..#..#..#....#.#..#.#..#.###..####.#..#.\n"
                 ".#...#..#....#.#..#.####.#....#..#.###..\n"
                 "#....#..#.#..#.#..#.#..#.#....#..#.#....\n"
                 "####..##...##...##..#..#.#....#..#.#....\n",
                 1000,
                 solve2);
}
