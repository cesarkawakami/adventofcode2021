#include <iostream>
#include <ios>
#include <vector>
#include <algorithm>

int count_increases(const std::vector<int> &vec) {
    int count = 0;
    for (int i = 0; i < std::ssize(vec) - 1; ++i) {
        if (vec[i] < vec[i + 1]) {
            count += 1;
        }
    }
    return count;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::vector<int> heights;
    std::copy(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              std::back_inserter(heights));
    std::cout << count_increases(heights) << '\n';

    for (int i = 0; i < std::ssize(heights) - 2; ++i) {
        heights[i] = heights[i] + heights[i + 1] + heights[i + 2];
    }
    heights.erase(heights.end() - 2, heights.end());
    std::cout << count_increases(heights) << '\n';
}
