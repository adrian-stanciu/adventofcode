#include <bits/stdc++.h>

using namespace std;

template <size_t N>
using Point = array<int, N>;

template <size_t N>
struct PointHasher {
    auto operator()(const Point<N>& p) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        for (auto i : p)
            h ^= i + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

template <size_t N>
using Map = unordered_map<Point<N>, bool, PointHasher<N>>;

void fill_border(const Map<3>& map, const Point<3>& p, Map<3>& new_map)
{
    for (auto x = -1; x <= 1; ++x)
        for (auto y = -1; y <= 1; ++y)
            for (auto z = -1; z <= 1; ++z) {
                auto neigh{p};
                neigh[0] += x;
                neigh[1] += y;
                neigh[2] += z;

                auto it = map.find(neigh);
                if (it == end(map))
                    new_map[neigh] = false;
                else
                    new_map[neigh] = it->second;
            }
}

void fill_border(const Map<4>& map, const Point<4>& p, Map<4>& new_map)
{
    for (auto x = -1; x <= 1; ++x)
        for (auto y = -1; y <= 1; ++y)
            for (auto z = -1; z <= 1; ++z)
                for (auto w = -1; w <= 1; ++w) {
                    auto neigh{p};
                    neigh[0] += x;
                    neigh[1] += y;
                    neigh[2] += z;
                    neigh[3] += w;

                    auto it = map.find(neigh);
                    if (it == end(map))
                        new_map[neigh] = false;
                    else
                        new_map[neigh] = it->second;
                }
}

template <typename T>
void fill_border(T& map)
{
    T new_map;

    for (auto [key, _] : map)
        fill_border(map, key, new_map);

    map.swap(new_map);
}

auto count_neigh(const Map<3>& map, const Point<3>& p, Map<3>& new_map)
{
    auto cnt = 0;

    for (auto x = -1; x <= 1; ++x)
        for (auto y = -1; y <= 1; ++y)
            for (auto z = -1; z <= 1; ++z) {
                if (!x && !y && !z)
                    continue;

                auto neigh{p};
                neigh[0] += x;
                neigh[1] += y;
                neigh[2] += z;

                auto it = map.find(neigh);
                if (it == end(map))
                    new_map[neigh] = false;
                else
                    cnt += it->second;
            }

    return cnt;
}

auto count_neigh(const Map<4>& map, const Point<4>& p, Map<4>& new_map)
{
    auto cnt = 0;

    for (auto x = -1; x <= 1; ++x)
        for (auto y = -1; y <= 1; ++y)
            for (auto z = -1; z <= 1; ++z)
                for (auto w = -1; w <= 1; ++w) {
                    if (!x && !y && !z && !w)
                        continue;

                    auto neigh{p};
                    neigh[0] += x;
                    neigh[1] += y;
                    neigh[2] += z;
                    neigh[3] += w;

                    auto it = map.find(neigh);
                    if (it == end(map))
                        new_map[neigh] = false;
                    else
                        cnt += it->second;
                }

    return cnt;
}

template <typename T>
auto solve(T& map, int num_iters)
{
    fill_border(map);

    for (auto i = 0; i < num_iters; ++i) {
        T new_map;

        for (auto [key, val] : map) {
            auto n = count_neigh(map, key, new_map);

            if (val)
                new_map[move(key)] = n == 2 || n == 3;
            else
                new_map[move(key)] = n == 3;
        }

        new_map.swap(map);
    }

    auto cnt = 0;
    for (auto [_, val] : map)
        cnt += val;
    return cnt;
}

int main()
{
    Map<3> map3d;
    Map<4> map4d;

    string line;
    for (auto y = 0; getline(cin, line); ++y)
        for (auto x = 0; x < static_cast<int>(line.size()); ++x) {
            auto val = line[x] == '#';
            map3d[{x, y, 0}] = val;
            map4d[{x, y, 0, 0}] = val;
        }

    cout << solve(map3d, 6) << '\n';
    cout << solve(map4d, 6) << '\n';

    return 0;
}

