#include "utils.h"
#include <queue>

const int MAXINT = std::numeric_limits<int>::max() / 2 - 10;
const std::vector<std::tuple<int, int>> DIRS{{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

struct Matrix {
    int R, C;
    std::vector<int> data;
    Matrix(int R, int C) : R(R), C(C), data(R * C) {}
    int &operator[](const std::tuple<int, int> &p) {
        const auto &[a, b] = p;
        return data[a * C + b];
    }
    const int &operator[](const std::tuple<int, int> &p) const {
        const auto &[a, b] = p;
        return data[a * C + b];
    }
};

auto solve2 = [](std::istream &is, std::ostream &os) {
    std::vector<std::string> lines;
    std::copy(std::istream_iterator<std::string>(is),
              std::istream_iterator<std::string>(),
              std::back_inserter(lines));

    int MR = std::ssize(lines), MC = std::ssize(lines[0]);
    int R = MR * 5, C = MC * 5;

    auto valid = [=](int r, int c) { return 0 <= r && r < R && 0 <= c && c < C; };

    Matrix matrix(R, C);
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            int x = lines[r % MR][c % MC] - '0' + r / MR + c / MC;
            while (x > 9) {
                x -= 9;
            }
            matrix[{r, c}] = x;
        }
    }

    Matrix dists(R, C);
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            dists[{r, c}] = MAXINT;
        }
    }
    Matrix visited(R, C);
    dists[{0, 0}] = 0;
    using tiii = std::tuple<int, int, int>;
    std::priority_queue<tiii, std::vector<tiii>, std::greater<tiii>> queue;
    queue.push({0, 0, 0});
    while (!queue.empty()) {
        const auto [curd, curr, curc] = queue.top();
        queue.pop();
        if (visited[{curr, curc}]) {
            continue;
        }
        visited[{curr, curc}] = true;
        // std::cerr << "D: " << curd << " " << curr << " " << curc << "\n";
        for (const auto &[dr, dc] : DIRS) {
            int nr = curr + dr, nc = curc + dc;
            if (!valid(nr, nc)) {
                continue;
            }
            int nd = curd + matrix[{nr, nc}];
            // std::cerr << "D1: " << curr << " " << curc << " " << dr << " " << dc << "\n";
            // std::cerr << "D2: " << nd << " " << nr << " " << nc << "\n";
            if (nd < dists[{nr, nc}]) {
                dists[{nr, nc}] = nd;
                queue.push({nd, nr, nc});
            }
        }
    }

    os << dists[{R - 1, C - 1}];
};

int main() {
    {
        utils::Tester tester;
        tester.test("part2", "in1", "315", solve2);
        tester.test("part2", "in2", "", solve2);
    }

    // utils::bench("part2", "in2", "", 50000, solve2);
}
