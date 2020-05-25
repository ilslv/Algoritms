#include <fstream>
#include <vector>

int main() {
    std::ifstream fin("concert.in");
    std::ofstream fout("concert.out");

    int m, n;
    long long d;

    fin >> n >> m >> d;

    std::vector<std::pair<int, long long>> optimal(n + 1, {-1, -1});
    std::vector<long long> songs;

    songs.reserve(n);
    for (int i = 0; i < n; i++) {
        long long tmp;
        fin >> tmp;
        songs.push_back(tmp);
    }

    optimal[0] = std::move(std::pair(m, 0));
    for (auto &song: songs) {
        for (int i = n; i > 0; i--) {
            if (optimal[i - 1].second >= song || (optimal[i - 1].first > 0 && d >= song)) {
                std::pair current = optimal[i - 1].second >= song ?
                                    std::pair(optimal[i - 1].first, optimal[i - 1].second - song) :
                                    std::pair(optimal[i - 1].first - 1, d - song);
                if  (current.first > optimal[i].first ||
                     (current.first == optimal[i].first && current.second > optimal[i].second)) {
                    optimal[i] = std::move(current);
                }
            }
        }
    }

    for (int i = n; i >= 0; i--) {
        if (optimal[i].first != -1) {
            fout << i;
            break;
        }
    }

    return 0;
}