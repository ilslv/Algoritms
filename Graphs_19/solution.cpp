#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <tuple>
#include <limits>

long long gcd(long long a, long long b) {
    if (a < b) {
        std::swap(a, b);
    }
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int n, m;
    fin >> n >> m;

    std::vector<std::vector<std::tuple<int, long long, int>>> list(n);

    int r1, r2, t1, t2;
    for (int i = 0; i < m; i++) {
        fin >> r1 >> t1 >> r2 >> t2;
        long long dist = lcm(t1, t2);
        list[r1 - 1].emplace_back(r2 - 1, dist, i + 1);
        list[r2 - 1].emplace_back(r1 - 1, dist, i + 1);
    }

    std::vector<std::tuple<double, int, int>> dist(n, {std::numeric_limits<double>::max(), 0, 0});
    std::vector<bool> visited(n);
    std::set<std::pair<double, int>> min_dist;

    dist[0] = {0, 0, 0};
    min_dist.emplace(0, 0);

    while (!min_dist.empty()) {
        auto[cur_dist, cur_vertex] = *min_dist.begin();
        min_dist.erase(min_dist.begin());
        visited[cur_vertex] = true;

        for (auto&&[vertex_to, dist_to, module] : list[cur_vertex]) {
            if (visited[vertex_to]) {
                continue;
            }

            double new_dist = 0.5 + dist_to + floor(cur_dist / dist_to) * dist_to;

            if (new_dist < std::get<0>(dist[vertex_to])) {
                min_dist.erase({std::get<0>(dist[vertex_to]), vertex_to});
                dist[vertex_to] = {new_dist, cur_vertex, module};
                min_dist.emplace(new_dist, vertex_to);
            }
        }
    }

    fout.precision(std::numeric_limits<double>::max_digits10);
    fout << std::get<0>(dist[n - 1]) << '\n';

    std::vector<int> path;
    int cur_vertex = n - 1;
    while (cur_vertex != 0) {
        path.push_back(std::get<2>(dist[cur_vertex]));
        cur_vertex = std::get<1>(dist[cur_vertex]);
    }

    for (int i = path.size() - 1; i >= 0; i--) {
        fout << path[i] << ' ';
    }

    return 0;
}