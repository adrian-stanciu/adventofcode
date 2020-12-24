#include <bits/stdc++.h>

using namespace std;

using int128 = __int128;

std::ostream& operator<<(std::ostream& out, int128 i)
{
    std::string s;

    while (i) {
        s.push_back(static_cast<char>((i % 10) + '0'));
        i /= 10;
    }

    reverse(begin(s), end(s));

    out << s;

    return out;
}

auto solve1(long start_t, const vector<pair<long, long>>& ids)
{
    auto min_dt = numeric_limits<long>::max();
    auto min_dt_id = 0;

    for (auto [id, _] : ids) {
        auto dt = id - start_t % id;

        if (dt < min_dt) {
            min_dt = dt;
            min_dt_id = id;
        }
    }

    return min_dt * min_dt_id;
}

struct gcd_result {
    int128 gcd;
    int128 x;
    int128 y;
};

// a * x + b * y = gcd(a, b)
auto gcd_extended(int128 a, int128 b)
{
    if (b == 0)
        return gcd_result{a, 1, 0};

    auto [gcd, x, y] = gcd_extended(b, a % b);

    return gcd_result{gcd, y, x - (a / b) * y};
}

auto solve2(const vector<pair<long, long>>& ids)
{
    queue<pair<int128, int128>> q;

    for (auto [id, pos] : ids)
        q.emplace(id, ((id - pos) % id + id) % id);

    while (q.size() > 1) {
        auto [a, rem_a] = q.front();
        q.pop();
        auto [b, rem_b] = q.front();
        q.pop();

        auto [gcd, x, y] = gcd_extended(a, b);
        assert(gcd == 1); // the chinese remainder theorem requires that a and b are coprime

        auto p = a * b;
        q.emplace(p, ((rem_a * b * y + rem_b * a * x) % p + p) % p);
    }

    return q.front().second;
}

int main()
{
    long start_t;
    cin >> start_t;

    vector<pair<long, long>> ids;

    string line;
    auto pos = 0;
    while (getline(cin, line, ',')) {
        if (line != "x") {
            auto id = strtol(line.data(), nullptr, 10);
            ids.emplace_back(id, pos);
        }
        ++pos;
    }

    cout << solve1(start_t, ids) << '\n';
    cout << solve2(ids) << '\n';

    return 0;
}

