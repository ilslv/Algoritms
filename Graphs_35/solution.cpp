#include <fstream>
#include <vector>

bool dfs(const std::vector<std::vector<std::pair<int, int>>> &list, std::vector<bool> &vertices,
         std::vector<int> &edges, int vertex, bool orienting) {
    vertices[vertex] = true;

    for (const auto&[vertex_to, edge] : list[vertex]) {
        if (!orienting) {
            if (edges[edge]) {
                continue;
            }
            edges[edge] = -1;

            if (vertices[vertex_to]) {
                dfs(list, vertices, edges, vertex_to, true);
                return true;
            } else {
                if (dfs(list, vertices, edges, vertex_to, false)) {
                    return true;
                }
            }
        } else {
            if (edges[edge] <= 0) {
                edges[edge] = vertex_to + 1;
                dfs(list, vertices, edges, vertex_to, true);
            }
        }
    }

    return orienting;
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int r, c;
    fin >> r >> c;

    std::vector<std::vector<std::pair<int, int>>> list(r);
    std::vector<bool> vertices(r);
    std::vector<int> edges(c);

    int v1, v2;
    for (int i = 0; i < c; i++) {
        fin >> v1 >> v2;
        list[v1 - 1].emplace_back(v2 - 1, i);
        list[v2 - 1].emplace_back(v1 - 1, i);
    }

    for (int i = 0; i < r; i++) {
        if (!vertices[i]) {
            if (!dfs(list, vertices, edges, i, false)) {
                fout << "No";
                return 0;
            }
        }
    }

    for (auto &e : edges) {
        fout << e << ' ';
    }

    return 0;
}