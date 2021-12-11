#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct Point {
    int x, y;

    auto operator<=>(const Point &a) const = default;

    Point operator-(const Point &a) const { return {x - a.x, y - a.y}; }
    Point &operator-=(const Point &a) { return *this = *this - a; }
    Point operator+(const Point &a) const { return {x + a.x, y + a.y}; }
    Point &operator+=(const Point &a) { return *this = *this + a; }
    Point operator/(int d) const { return {x / d, y / d}; }
};

struct Line {
    Point a, b;

    bool operator<=>(const Line &a) const = default;

    void for_each_coord(auto fn) const {
        if (a == b) {
            fn(a);
            return;
        }
        Point delta = b - a;
        int gcd = std::gcd(delta.x, delta.y);
        Point delta_step = delta / gcd;
        for (Point p = a;; p += delta_step) {
            fn(p);
            if (p == b) {
                break;
            }
        }
    }

    bool is_axis_aligned() const {
        return a.x == b.x || a.y == b.y;
    }

    static std::optional<Line> read() {
        std::string input_line;
        std::getline(std::cin, input_line);
        if (!std::cin) {
            return std::nullopt;
        }
        input_line = std::regex_replace(input_line, std::regex{"[,\\->]"}, " ");
        int ax, ay, bx, by;
        std::istringstream input_line_ss{input_line};
        input_line_ss >> ax >> ay >> bx >> by;
        if (!input_line_ss) {
            return std::nullopt;
        }
        return Line{{ax, ay}, {bx, by}};
    }
};

int main() {
    std::vector<Line> lines;
    while (true) {
        if (auto line = Line::read()) {
            // std::cout << "DEBUG: " << line->a.x << " " << line->a.y << "  " << line->b.x << " " << line->b.y << "\n";
            lines.push_back(*line);
        } else {
            break;
        }
    }
    // std::cout << "finished input" << "\n";

    std::initializer_list<std::tuple<std::string, bool>> cases = {{"part 1", true},
                                                                  {"part 2", false}};
    for (const auto &[test_title, needs_axis_aligned] : cases) {
        std::map<Point, int> coord_counts;
        for (const auto &line : lines) {
            if (!needs_axis_aligned || line.is_axis_aligned()) {
                line.for_each_coord([&](Point p) {
                    coord_counts[p] += 1;
                });
            }
        }

        int intersection_count = std::count_if(coord_counts.begin(), coord_counts.end(),
                                               [](auto pair) { const auto [coord, count] = pair; return count >= 2; });
        std::cout << test_title << " answer: " << intersection_count << '\n';
    }
}
