#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

const int N = 5;

struct Board {
    int data[N][N];

    bool operator==(const Board &) const = default;

    bool is_won(const std::set<int> &drawn) const {
        for (int i = 0; i < N; ++i) {
            auto check = [&](auto fn) {
                bool all_drawn = true;
                for (int j = 0; j < N; ++j) {
                    if (!drawn.contains(fn(i, j))) {
                        all_drawn = false;
                        break;
                    }
                }
                return all_drawn;
            };
            if (check([&](int i, int j) { return data[i][j]; }) ||
                check([&](int i, int j) { return data[j][i]; })) {
                return true;
            }
        }
        return false;
    }

    int score(const std::set<int> &drawn, int last) const {
        int unmarked_sum = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!drawn.contains(data[i][j])) {
                    unmarked_sum += data[i][j];
                }
            }
        }
        return unmarked_sum * last;
    }

    static Board read_board() {
        Board board;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!(std::cin >> board.data[i][j])) {
                    return {};
                }
            }
        }
        return board;
    }
};

std::vector<std::tuple<int, int, int>> run_draw(const std::vector<Board> &boards,
                                                const std::vector<int> &numbers) {
    std::vector<std::tuple<int, int, int>> rv;
    std::set<int> drawn;
    std::vector<int8_t> already_won(std::size(boards));
    for (int currently_drawn : numbers) {
        drawn.insert(currently_drawn);
        for (int i = 0; i < std::ssize(boards); ++i) {
            if (!already_won[i] && boards[i].is_won(drawn)) {
                rv.push_back({i, currently_drawn, boards[i].score(drawn, currently_drawn)});
                already_won[i] = true;
            }
        }
    }
    return rv;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string numbers_line;
    std::getline(std::cin, numbers_line);
    numbers_line = std::regex_replace(numbers_line, std::regex{","}, " ");

    std::vector<int> numbers;
    std::istringstream numbers_stream{numbers_line};
    std::copy(std::istream_iterator<int>(numbers_stream),
              std::istream_iterator<int>(),
              std::back_inserter(numbers));

    std::vector<Board> boards;
    while (true) {
        Board board = Board::read_board();
        if (board == Board{}) {
            break;
        }
        boards.push_back(board);
    }

    std::vector<std::tuple<int, int, int>> draw_results = run_draw(boards, numbers);
    for (const auto &[board_idx, last_drawn, score] : draw_results) {
        std::cout << "DEBUG: " << board_idx << " " << last_drawn << " " << score << "\n";
    }
    auto [_1, _2, first_won_score] = *draw_results.begin();
    auto [_3, _4, last_won_score] = *draw_results.rbegin();
    std::cout << "first won scroe: " << first_won_score << '\n';
    std::cout << "last won scroe:  " << last_won_score << '\n';
}
