#include <fstream>
#include <vector>

void input(int &size, std::vector<bool> &x, std::ifstream &fin) {
    bool first_one = false;

    char c;
    int j = 0;
    for (int i = 0; i < size; i++) {
        fin >> c;

        if (c == '0' && first_one) {
            j++;
        } else if (c == '1') {
            first_one = true;
            x[j] = true;
            j++;
        }
    }

    size = j;
}

std::pair<int, int> common_begin_end(std::vector<bool> &x, int n, std::vector<bool> &y, int m) {
    auto res = std::pair{0, 0};

    int x_offset = x.size() - n;
    int y_offset = y.size() - m;

    for (auto x_iter = x.cbegin(), y_iter = y.cbegin();
         x_iter != x.cend() - x_offset && y_iter != y.cend() - y_offset && *x_iter == *y_iter;
         x_iter++, y_iter++) {
        res.first++;
    }

    for (auto x_iter = x.crbegin() + x_offset, y_iter = y.crbegin() + y_offset;
         x_iter != x.crend() - res.first && y_iter != y.crend() - res.first && *x_iter == *y_iter;
         x_iter++, y_iter++) {
        res.second++;
    }

    return res;
}

bool cmp(const std::vector<bool> &lh, const std::vector<bool> &rh) {
    if (lh.size() != rh.size()) {
        return lh.size() < rh.size();
    }

    int i = 0;
    while (i < lh.size() && lh[i] == rh[i]) {
        i++;
    }

    if (i == lh.size()) {
        return true;
    }

    return rh[i];
}

std::vector<bool> fill(int n, int m, std::vector<bool> &&x, std::vector<bool> &&y) {
    std::vector<std::vector<bool>> cur_row(m + 1), prev_row(m + 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (x[i] == y[j]) {
                cur_row[j + 1] = std::move(prev_row[j]);
                cur_row[j + 1].push_back(x[i]);
            } else {
                cur_row[j + 1] = std::max(cur_row[j], prev_row[j + 1], cmp);
            }
        }
        std::swap(cur_row, prev_row);
    }

    return prev_row[m];
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int n, m;
    fin >> n >> m;

    std::vector<bool> x(n), y(m);

    input(n, x, fin);
    input(m, y, fin);

    auto&&[begin_offset, end_offset] = common_begin_end(x, n, y, m);

    auto result = fill(n - (begin_offset + end_offset), m - (begin_offset + end_offset),
                       std::vector<bool>(x.begin() + begin_offset, x.end() - end_offset),
                       std::vector<bool>(y.begin() + begin_offset, y.end() - end_offset));

    fout << result.size() + begin_offset + end_offset << '\n';
    for (int i = 0; i < begin_offset; i++) {
        fout << (x[i] ? '1' : '0');
    }

    for (auto &&e : result) {
        fout << (e ? '1' : '0');
    }

    for (int i = n - end_offset; i < n; i++) {
        fout << (x[i] ? '1' : '0');
    }

    return 0;
}