#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// weighted quick-union with path halving
struct UnionFind {
private:
    std::vector<int> ids;
    std::vector<int> sizes;
    size_t size;

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
        for (auto i = 0U; i < capacity; ++i)
            ids[i] = i;
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
};

struct Point {
    int x;
    int y;
    int z;
    int t;
};

auto dist(const Point& p1, const Point& p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z) + abs(p1.t - p2.t);
}

auto count_constellations(std::vector<Point> points)
{
    UnionFind uf(points.size());

    for (auto i = 0U; i < points.size() - 1; ++i)
        for (auto j = i + 1; j < points.size(); ++j)
            if (dist(points[i], points[j]) <= 3)
                uf.connect(i, j);

    return uf.get_size();
}

int main()
{
    std::vector<Point> points;

    std::string line;
    while (getline(std::cin, line)) {
        int x, y, z, t;
        sscanf(line.data(), "%d,%d,%d,%d", &x, &y, &z, &t);

        points.push_back({x, y, z, t});
    }

    std::cout << count_constellations(points) << "\n";

    return 0;
}

