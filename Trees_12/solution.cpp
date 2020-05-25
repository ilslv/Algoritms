#include <memory>
#include <functional>
#include <fstream>
#include <vector>

class tree {
public:
    struct node {
        int key;
        int height, max_path, min_h, min_h1;
        bool is_in_final_path = false;
        std::shared_ptr<node> left = nullptr;
        std::shared_ptr<node> right = nullptr;

        explicit node(int key) : key(key) {}
    };

    struct path {
        std::shared_ptr<node> root;
        int left_end, right_end;
    };

    std::shared_ptr<node> root = nullptr;
    std::vector<std::shared_ptr<node>> possible_roots;
    int max_path = 0;

    void left_traversal(const std::function<void(std::shared_ptr<node> &node)> &handler, std::shared_ptr<node> &node) {
        if (node != nullptr) {
            handler(node);
            left_traversal(handler, node->left);
            left_traversal(handler, node->right);
        }
    }

    void insert(const int key) {
        auto current = &root;
        while (*current != nullptr) {
            auto &node = **current;
            if (key > (*current)->key) {
                current = &node.right;
            } else if (key < (*current)->key) {
                current = &node.left;
            } else {
                return;
            }
        }
        *current = std::make_shared<node>(key);
    }

    void foreach(const std::function<void(std::shared_ptr<node> &node)> &handler) {
        left_traversal(handler, root);
    }

    void remove(int key) {
        auto current = &root;
        while (*current != nullptr && (*current)->key != key) {
            if (key > (*current)->key) {
                current = &(*current)->right;
            } else {
                current = &(*current)->left;
            }
        }

        if (*current == nullptr) {
            return;
        }

        if ((*current)->left != nullptr && (*current)->right != nullptr) {
            auto min = &(*current)->right;
            while ((*min)->left != nullptr) {
                min = &(*min)->left;
            }
            (*current)->key = (*min)->key;
            *min = (*min)->right != nullptr ? (*min)->right : (*min)->left;
        } else {
            *current = (*current)->right != nullptr ? (*current)->right : (*current)->left;
        }
    }

    void calculate_properties(std::shared_ptr<node> &node) {
        if (node->left == nullptr && node->right == nullptr) {
            node->height = 0;
            node->max_path = 0;
            node->min_h = node->key;
            return;
        }

        if (node->right == nullptr || node->left == nullptr) {
            auto child = node->right != nullptr ? node->right : node->left;

            calculate_properties(child);
            node->height = child->height + 1;
            node->max_path = node->height;
            node->min_h = child->min_h;
            if (node->height < 2) {
                node->min_h1 = node->key;
            } else {
                node->min_h1 = child->min_h1;
            }
        } else {
            calculate_properties(node->left);
            calculate_properties(node->right);
            node->height = std::max(node->left->height, node->right->height) + 1;
            node->max_path = node->left->height + node->right->height + 2;
            if (node->left->height != node->right->height) {
                node->min_h = node->left->height > node->right->height ?
                              node->left->min_h : node->right->min_h;
            } else {
                node->min_h = std::min(node->left->min_h, node->right->min_h);
            }
            if (node->height < 2) {
                node->min_h1 = node->key;
            } else {
                if (node->left->height != node->right->height) {
                    node->min_h1 = node->left->height > node->right->height ?
                                   node->left->min_h1 : node->right->min_h1;
                } else {
                    node->min_h1 = std::min(node->left->min_h1, node->right->min_h1);
                }
            }
        }

        if (node->max_path > max_path) {
            max_path = node->max_path;
            while (!possible_roots.empty()) {
                possible_roots.pop_back();
            }
            possible_roots.push_back(node);
        } else if (node->max_path == max_path) {
            possible_roots.push_back(node);
        }
    }

    void mark_final_path(std::shared_ptr<node> &node, int key) {
        node->is_in_final_path = false;
        if (node->right == nullptr && node->left == nullptr) {
            node->is_in_final_path = node->key == key;
        } else if (node->right == nullptr || node->left == nullptr) {
            auto child = node->left != nullptr ? node->left : node->right;
            mark_final_path(child, key);
            node->is_in_final_path = child->is_in_final_path;
        } else {
            mark_final_path(node->left, key);
            mark_final_path(node->right, key);
            node->is_in_final_path = node->right->is_in_final_path || node->left->is_in_final_path;
        }
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> min_left_right(std::shared_ptr<node> &node) {
        if (node->right == nullptr || node->left == nullptr) {
            auto child = node->right != nullptr ? node->right : node->left;
            if (child->min_h < node->key) {
                return {{child->min_h, node->key}, {0, 0}};
            }
            return {{node->key, child->min_h}, {0, 0}};
        } else {
            int l_min_h, l_min_h1, r_min_h, r_min_h1;
            if (node->left->left == nullptr && node->left->right == nullptr) {
                l_min_h = node->left->key;
                l_min_h1 = node->key;
            } else {
                l_min_h = node->left->min_h;
                l_min_h1 = node->left->min_h1;
            }
            if (node->right->left == nullptr && node->right->right == nullptr) {
                r_min_h = node->right->key;
                r_min_h1 = node->key;
            } else {
                r_min_h = node->right->min_h;
                r_min_h1 = node->right->min_h1;
            }

            if (l_min_h + r_min_h1 < l_min_h1 + r_min_h) {
                return {{l_min_h, r_min_h1}, {0, 0}};
            } else if (l_min_h + r_min_h1 > l_min_h1 + r_min_h) {
                return {{l_min_h1, r_min_h}, {0, 0}};
            } else {
                return {{l_min_h, r_min_h1}, {l_min_h1, r_min_h}};
            }
        }
    }

    std::shared_ptr<node> find_median(path &path) {
        if (max_path % 2 == 0) {
            return nullptr;
        }

        int left_height, right_height;

        if (path.left_end == path.root->key || path.right_end == path.root->key) {
            left_height = path.left_end == path.root->key ? -1 : max_path - 2;
            right_height = path.left_end == path.root->key ? max_path - 2 : -1;
        } else {
            left_height = path.root->left->height;
            right_height = path.root->right->height;
            if (path.root->left->min_h != path.left_end) {
                left_height--;
            } else {
                right_height--;
            }
            if (left_height == right_height) {
                return path.root;
            }
        }
        auto child = left_height > right_height ? path.root->left : path.root->right;
        mark_final_path(child, child->min_h);
        int left_pointer = 0, right_pointer = 0;

        if (path.root->left == child) {
            right_pointer = right_height + 2;
        } else {
            left_pointer = left_height + 2;
        }

        auto cur_node = &child;
        int middle = max_path / 2;
        while (true) {
            if ((*cur_node)->left != nullptr && (*cur_node)->left->is_in_final_path) {
                right_pointer++;
                if (right_pointer > middle) {
                    break;
                }
                cur_node = &(*cur_node)->left;
            } else {
                left_pointer++;
                if (left_pointer > middle) {
                    break;
                }
                cur_node = &(*cur_node)->right;
            }
        }

        return *cur_node;
    }

    void find_result() {
        int min_sum = INT_MAX;
        int min_root = INT_MAX;
        int cur_sum;
        std::vector<path> paths;

        for (auto &node : possible_roots) {
            auto ends = min_left_right(node);
            cur_sum = ends.first.first + ends.first.second;
            if (cur_sum < min_sum) {
                min_sum = cur_sum;
                min_root = node->key;
                while (!paths.empty()) {
                    paths.pop_back();
                }
                paths.push_back(path{node, ends.first.first, ends.first.second});
                if (ends.second.first != 0 && ends.second.second != 0) {
                    paths.push_back(path{node, ends.second.first, ends.second.second});
                }
            } else if (cur_sum == min_sum) {
                if (node->key < min_root) {
                    min_root = node->key;
                    while (!paths.empty()) {
                        paths.pop_back();
                    }
                    paths.push_back(path{node, ends.first.first, ends.first.second});
                    if (ends.second.first != 0 && ends.second.second != 0) {
                        paths.push_back(path{node, ends.second.first, ends.second.second});
                    }
                }
            }
        }

        std::vector<std::shared_ptr<node>> medians;
        for (auto &path : paths) {
            auto median = find_median(path);
            if (median != nullptr) {
                medians.push_back(median);
            } else {
                return;
            }
        }

        if (!medians.empty()) {
            for (int i = 0; i < medians.size() - 1; i++) {
                if (medians[i] != medians[i + 1]) {
                    return;
                }
            }
            remove(medians[0]->key);
        }
    }

    void solve() {
        if (root == nullptr || (root->left == nullptr && root->right == nullptr)) {
            return;
        }

        calculate_properties(root);
        for (auto &element : possible_roots) {
            if (element == root && (root->left == nullptr || root->right == nullptr)) {
                max_path++;
                while (!possible_roots.empty()) {
                    possible_roots.pop_back();
                }
            }
        }
        if (root->max_path == max_path - 1 && (root->left == nullptr || root->right == nullptr)) {
            possible_roots.push_back(root);
        }
        find_result();
    }
};

int main() {
    std::ifstream fin("tst.in");
    std::ofstream fout("tst.out");

    tree tree;

    int key;
    while (fin >> key) {
        tree.insert(key);
    }

    tree.solve();

    tree.foreach([&fout](std::shared_ptr<tree::node> &node) -> void {
        fout << node->key << '\n';
    });

    return 0;
}