#include <fstream>
#include <vector>
#include <algorithm>

struct node {
    int min = 0;
    int max = 0;
    int sum = 0;
    int add = 0;
};

class segment_tree {
private:
    std::vector<node> tree;
    int n;

    void add(int value, int begin, int end, int l, int r, int cur_node) {
        if (begin == l && end == r) {
            tree[cur_node].add += value;
        } else {
            tree[cur_node].sum += value * (end - begin + 1);
            int m = (l + r) / 2;
            if (begin <= m && m < end) {
                add(value, begin, m, l, m, cur_node * 2 + 1);
                add(value, m + 1, end, m + 1, r, cur_node * 2 + 2);
            } else if (m < begin) {
                add(value, begin, end, m + 1, r, cur_node * 2 + 2);
            } else {
                add(value, begin, end, l, m, cur_node * 2 + 1);
            }

            tree[cur_node].min = std::min(tree[cur_node * 2 + 1].add + tree[cur_node * 2 + 1].min,
                                          tree[cur_node * 2 + 2].add + tree[cur_node * 2 + 2].min);
            tree[cur_node].max = std::max(tree[cur_node * 2 + 1].add + tree[cur_node * 2 + 1].max,
                                          tree[cur_node * 2 + 2].add + tree[cur_node * 2 + 2].max);
        }
    }

    void change(int value, int id, int l, int r, int cur_node) {
        value -= tree[cur_node].add;
        if (l == r) {
            add(value, l, l, 0, n - 1, 0);
        } else {
            int m = (l + r) / 2;
            if (id <= m) {
                change(value, id, l, m, cur_node * 2 + 1);
            } else {
                change(value, id, m + 1, r, cur_node * 2 + 2);
            }
        }
    }

    int find_sum(int begin, int end, int l, int r, int cur_node) {
        int add_val = tree[cur_node].add * (end - begin + 1);
        if (begin == l && end == r) {
            return add_val + tree[cur_node].sum;
        }
        int m = (l + r) / 2;
        if (begin <= m && m < end) {
            return add_val +
                   find_sum(begin, m, l, m, cur_node * 2 + 1) +
                   find_sum(m + 1, end, m + 1, r, cur_node * 2 + 2);
        }
        if (m < begin) {
            return add_val + find_sum(begin, end, m + 1, r, cur_node * 2 + 2);
        }
        return add_val + find_sum(begin, end, l, m, cur_node * 2 + 1);
    }

    int find_min(int begin, int end, int l, int r, int cur_node) {
        if (begin == l && end == r) {
            return tree[cur_node].add + tree[cur_node].min;
        }
        int m = (l + r) / 2;
        if (begin <= m && m < end) {
            return tree[cur_node].add +
                   std::min(find_min(begin, m, l, m, cur_node * 2 + 1),
                            find_min(m + 1, end, m + 1, r, cur_node * 2 + 2));
        }
        if (m < begin) {
            return tree[cur_node].add + find_min(begin, end, m + 1, r, cur_node * 2 + 2);
        }
        return tree[cur_node].add + find_min(begin, end, l, m, cur_node * 2 + 1);
    }

    int find_max(int begin, int end, int l, int r, int cur_node) {
        if (begin == l && end == r) {
            return tree[cur_node].add + tree[cur_node].max;
        }
        int m = (l + r) / 2;
        if (begin <= m && m < end) {
            return tree[cur_node].add +
                   std::max(find_max(begin, m, l, m, cur_node * 2 + 1),
                            find_max(m + 1, end, m + 1, r, cur_node * 2 + 2));
        }
        if (m < begin) {
            return tree[cur_node].add + find_max(begin, end, m + 1, r, cur_node * 2 + 2);
        }
        return tree[cur_node].add + find_max(begin, end, l, m, cur_node * 2 + 1);
    }

public:
    explicit segment_tree(int n) {
        tree.resize(4 * n);
        this->n = n;
    }

    void change(int value, int id) {
        change(value, id, 0, n - 1, 0);
    }

    void add(int value, int begin, int end) {
        add(value, begin, end, 0, n - 1, 0);
    }

    int find_sum(int begin, int end) {
        return find_sum(begin, end, 0, n - 1, 0);
    }

    int find_min(int begin, int end) {
        return find_min(begin, end, 0, n - 1, 0);
    }

    int find_max(int begin, int end) {
        return find_max(begin, end, 0, n - 1, 0);
    }
};

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int n;
    fin >> n;

    segment_tree tree(n);

    int operation;
    while (fin >> operation) {
        int arg1, arg2, arg3;
        switch (operation) {
            case 0:
                break;
            case 1:
                fin >> arg1 >> arg2;
                tree.change(arg2, arg1);
                break;
            case 2:
                fin >> arg1 >> arg2 >> arg3;
                tree.add(arg3, arg1, arg2);
                break;
            case 3:
                fin >> arg1 >> arg2;
                fout << tree.find_sum(arg1, arg2) << '\n';
                break;
            case 4:
                fin >> arg1 >> arg2;
                fout << tree.find_min(arg1, arg2) << '\n';
                break;
            case 5:
                fin >> arg1 >> arg2;
                fout << tree.find_max(arg1, arg2) << '\n';
                break;
        }
    }

    return 0;
}