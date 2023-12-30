#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

struct Point {
    long z;
    long x;
    long y;

    auto operator<=>(const Point&) const = default;
};

struct Seg {
    Point from;
    Point to;
    int idx;

    Seg(Point from, Point to, int idx) : from{from}, to{to}, idx{idx} {}

    auto operator<=>(const Seg&) const = default;

    [[nodiscard]] bool is_on(const Seg& other) const
    {
        auto intersect = [](auto lhs_from, auto lhs_to, auto rhs_from, auto rhs_to) {
            return max(lhs_from, rhs_from) <= min(lhs_to, rhs_to);
        };

        return intersect(from.x, to.x, other.from.x, other.to.x) &&
            intersect(from.y, to.y, other.from.y, other.to.y);
    }
};

auto count_dependant(vector<set<int>> below_vec, const vector<set<int>>& above_vec, int u)
{
    queue<int> q;
    q.emplace(u);

    auto cnt = 0;

    while (!q.empty()) {
        auto u = q.front();
        q.pop();

        for (auto v : above_vec[u]) {
            below_vec[v].erase(u);

            if (below_vec[v].empty()) {
                ++cnt;
                q.emplace(v);
            }
        }
    }

    return cnt;
}

void solve(int n, vector<Seg>& v)
{
    sort(begin(v), end(v));

    vector<set<int>> below_vec(n);
    vector<set<int>> above_vec(n);

    for (auto i = 0; i < n; ++i) {
        auto top_z = 0;

        for (auto j = i - 1; j >= 0; --j) {
            if (v[j].to.z < top_z)
                break;

            if (v[i].is_on(v[j])) {
                below_vec[v[i].idx].insert(v[j].idx);
                above_vec[v[j].idx].insert(v[i].idx);

                top_z = v[j].to.z;
            }
        }

        v[i].to.z = top_z + 1 + (v[i].to.z - v[i].from.z);
        v[i].from.z = top_z + 1;

        sort(begin(v), begin(v) + i + 1, [](const auto& lhs, const auto& rhs) {
            return tie(lhs.to, lhs.from, lhs.idx) < tie(rhs.to, rhs.from, rhs.idx);
        });
    }

    vector<bool> can_be_removed(n, true);
    for (auto& below : below_vec)
        if (ssize(below) == 1)
            can_be_removed[*begin(below)] = false;

    cout << count(begin(can_be_removed), end(can_be_removed), true) << '\n';

    auto cnt = 0;
    for (auto i = 0; i < n; ++i)
        if (!can_be_removed[i])
            cnt += count_dependant(below_vec, above_vec, i);
    cout << cnt << '\n';
}

int main()
{
    auto n = 0;
    vector<Seg> v;

    string s;
    while (getline(cin, s)) {
        string_view sv{s};

        auto sep = sv.find('~');
        auto src = split(sv.substr(0, sep), ',');
        auto dst = split(sv.substr(sep + 1), ',');

        Point from{str2num(src[2]), str2num(src[0]), str2num(src[1])};
        Point to{str2num(dst[2]), str2num(dst[0]), str2num(dst[1])};
        if (from > to)
            swap(from, to);

        v.emplace_back(from, to, n++);
    }

    solve(n, v);

    return 0;
}

