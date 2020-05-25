#include <fstream>
#include <vector>
#include <array>
#include <stack>
#include <tuple>

struct point {
    int x = 0;
    int y = 0;

    point operator+(const point &other) const {
        return {x + other.x, y + other.y};
    }

    bool operator!=(const point &other) const {
        return x != other.x || y != other.y;
    }
};

constexpr std::array<point, 4> directions{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

bool find_path(const point &begin, const point &end, std::vector<std::vector<bool>> &labyrinth) {
    std::stack<std::tuple<point, int, int>> path;

    point current(begin);
    int current_direction = 1;
    int current_offset = 0;

    while (current != end) {
        if (current_direction < 4) {
            point step = current + directions[(current_direction + current_offset) % 4];

            if (labyrinth[step.x][step.y] ||
                step.x == 1 ||
                (step.x == end.x && step.y != end.y)) {
                current_direction++;
            } else {
                path.emplace(current, current_direction, current_offset);
                labyrinth[current.x][current.y] = true;

                current = step;
                current_offset = (current_direction + current_offset + 3) % 4;
                current_direction = 0;
            }

        } else {
            if (path.empty()) {
                return false;
            }

            auto&[prev, prev_direction, prev_offset] = path.top();

            current = prev;
            current_direction = prev_direction + 1;
            current_offset = prev_offset;
            path.pop();
        }
    }

    labyrinth[end.x][end.y] = true;

    return true;
}

int main() {
    std::ifstream fin("in.txt");
    std::ofstream fout("out.txt");

    int n, m;
    fin >> n >> m;

    std::vector<std::vector<bool>> labyrinth(n + 2, std::vector<bool>(m + 2, true));
    std::vector<std::pair<point, point>> begin_end;

    char tmp;
    int end = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            fin >> tmp;
            if (tmp == '0') {
                labyrinth[i][j] = false;

                if (i == 1) {
                    begin_end.push_back({{i, j},
                                         {}});
                } else if (i == n) {
                    begin_end[end].second = {i, j};
                    end++;
                }
            }
        }
    }

    for (auto&[begin, end] : begin_end) {
        if (!find_path(begin, end, labyrinth)) {
            fout << "Impossible";
            return 0;
        }
    }

    fout << "Possible";

    return 0;
}