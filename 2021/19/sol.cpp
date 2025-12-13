#include <bits/stdc++.h>

using namespace std;

using Point = array<long, 3>;
using Scanner = vector<Point>;

auto generate_orientations(const Scanner& scanner)
{
    vector<Scanner> orientations(24);

    for (const auto& point : scanner) {
        auto idx = 0;

        auto add_orientations = [&](auto x, auto y, auto z) {
            // 4 90-degree yz rotations
            orientations[idx++].push_back(Point{x, y, z});
            orientations[idx++].push_back(Point{x, z, -y});
            orientations[idx++].push_back(Point{x, -y, -z});
            orientations[idx++].push_back(Point{x, -z, y});
        };

        for (auto k = 0; k < 3; ++k) {
            auto rotation{point};
            rotate(begin(rotation), begin(rotation) + k, end(rotation));

            auto x = rotation[0];
            auto y = rotation[1];
            auto z = rotation[2];

            // 2 180-degree xy rotations
            add_orientations(x, y, z);
            add_orientations(-x, -y, z);
        }
    }

    return orientations;
}

auto count_unique_points(const vector<Scanner>& scanners)
{
    set<Point> uniq;

    for (const auto& scanner : scanners)
        for (const auto& point : scanner)
            uniq.insert(point);

    return uniq.size();
}

auto find_max_manhattan_distance(const vector<Point>& positions)
{
    auto dist = [](const auto& p1, auto& p2) {
        auto d = 0;

        for (auto i = 0; i < 3; ++i)
            d += abs(p1[i] - p2[i]);

        return d;
    };

    auto max_d = 0;

    for (const auto& p1 : positions)
        for (const auto& p2 : positions)
            max_d = max(max_d, dist(p1, p2));

    return max_d;
}

auto solve(vector<Scanner>& scanners)
{
    auto n = static_cast<int>(scanners.size());

    vector<Point> scanner_positions(n);
    scanner_positions[0] = Point{0, 0, 0};

    auto are_overlapping = [&](auto i, auto j) {
        static const auto OverlappingThreshold = 12;

        for (auto& scanner_j : generate_orientations(scanners[j])) {
            map<Point, int> point2overlaps;

            auto max_overlaps = 0;
            auto remaining = scanners[i].size();

            for (const auto& point_i : scanners[i]) {
                if (max_overlaps + remaining < OverlappingThreshold)
                    break;

                for (const auto& point_j : scanner_j) {
                    Point scanner_j_pos;
                    for (auto k = 0; k < 3; ++k)
                        scanner_j_pos[k] = point_i[k] - point_j[k];

                    auto& num_overlaps = point2overlaps[scanner_j_pos];

                    if (++num_overlaps == OverlappingThreshold) {
                        scanner_positions[j] = scanner_j_pos;

                        for (auto& point_j : scanner_j)
                            for (auto k = 0; k < 3; ++k)
                                point_j[k] += scanner_j_pos[k];

                        scanners[j] = move(scanner_j);

                        return true;
                    }

                    max_overlaps = max(max_overlaps, num_overlaps);
                }

                --remaining;
            }
        }

        return false;
    };

    queue<int> q;
    q.push(0);

    vector<bool> visited(n, false);
    visited[0] = true;

    while (!q.empty()) {
        auto i = q.front();
        q.pop();

        for (auto j = 0; j < n; ++j)
            if (!visited[j] && are_overlapping(i, j)) {
                q.push(j);
                visited[j] = true;
            }
    }

    cout << count_unique_points(scanners) << '\n';
    cout << find_max_manhattan_distance(scanner_positions) << '\n';
}

int main()
{
    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    vector<Scanner> scanners;
    Scanner scanner;

    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            continue;
        } else if (line.back() == '-') {
            if (scanner.empty())
                continue;

            scanners.push_back(move(scanner));
            scanner.clear();
        } else {
            stringstream ss{line};

            string x;
            getline(ss, x, ',');
            string y;
            getline(ss, y, ',');
            string z;
            getline(ss, z, ',');

            scanner.push_back(Point{to_num(x), to_num(y), to_num(z)});
        }
    }

    if (!scanner.empty())
        scanners.push_back(move(scanner));

    solve(scanners);

    return 0;
}

