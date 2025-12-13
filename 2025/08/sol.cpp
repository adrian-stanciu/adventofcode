#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

struct Point {
    long x;
    long y;
    long z;

    Point(long x, long y, long z) : x{x}, y{y}, z{z} {}
};

// weighted quick-union with path halving
struct UnionFind {
private:
    std::vector<int> ids;
    std::vector<int> sizes;
    size_t size{0};

    [[nodiscard]] auto root(int i)
    {
        while (i != ids[i]) {
            ids[i] = ids[ids[i]];
            i = ids[i];
        }

        return i;
    }

public:
    UnionFind(size_t capacity)
    : ids(capacity, 0)
    , sizes(capacity, 1)
    , size(capacity)
    {
        iota(begin(ids), end(ids), 0);
    }

    [[nodiscard]] bool connected(int i1, int i2)
    {
        return root(i1) == root(i2);
    }

    void connect(int i1, int i2)
    {
        auto root1 = root(i1);
        auto root2 = root(i2);

        if (root1 == root2)
            return;

        if (sizes[root1] < sizes[root2]) {
            ids[root1] = root2;
            sizes[root2] += sizes[root1];
        } else {
            ids[root2] = root1;
            sizes[root1] += sizes[root2];
        }

        --size;
    }

    [[nodiscard]] size_t get_size() const
    {
        return size;
    }


    [[nodiscard]] vector<int> get_sizes() const
    {
        return sizes;
    }
};

auto compute_distances(const vector<Point>& v)
{
    auto square = [](auto x) {
        return x * x;
    };

    auto compute_sq_distance = [&](const auto& from, const auto& to) {
        return square(to.x - from.x) + square(to.y - from.y) + square(to.z - from.z);
    };

    auto n = ssize(v);

    vector<pair<long, pair<int, int>>> distances;
    distances.reserve(square(n) / 2);

    for (auto i = 0; i < n; ++i)
        for (auto j = i + 1; j < n; ++j)
            distances.emplace_back(compute_sq_distance(v[i], v[j]), make_pair(i, j));

    return distances;
}

auto solve(const vector<Point>& v, int iters)
{
    auto distances = compute_distances(v);
    sort(begin(distances), end(distances));

    UnionFind uf{v.size()};

    for (auto i = 0; i < iters; ++i) {
        const auto& [from, to] = distances[i].second;
        uf.connect(from, to);
    }

    auto sizes = uf.get_sizes();
    sort(rbegin(sizes), rend(sizes));
    cout << 1l * sizes[0] * sizes[1] * sizes[2] << '\n';

    for (auto i = iters; i < ssize(distances); ++i) {
        const auto& [from, to] = distances[i].second;
        uf.connect(from, to);
        if (uf.get_size() == 1) {
            cout << v[from].x * v[to].x << '\n';
            break;
        }
    }
}

int main()
{
    vector<Point> v;

    string s;
    while (getline(cin, s)) {
        auto coords = split(string_view{s}, ',');
        v.emplace_back(str2num(coords[0]), str2num(coords[1]), str2num(coords[2]));
    }

    solve(v, 1000);

    return 0;
}

