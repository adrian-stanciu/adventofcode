#include <bits/stdc++.h>

using namespace std;

auto play(vector<int>& v, int iters)
{
    auto curr = v[0];
    auto sz = ssize(v);
    vector<int> next(sz + 1);

    auto extract_chunk = [&] () {
        array<int, 3> chunk{
            next[curr],
            next[next[curr]],
            next[next[next[curr]]]
        };
        next[curr] = next[chunk.back()];
        return chunk;
    };

    for (auto i = 0; i < sz - 1; ++i)
        next[v[i]] = v[i + 1];
    next[v[sz - 1]] = v[0];

    for (auto i = 0; i < iters; ++i) {
        auto chunk = extract_chunk();

        auto dst = curr;
        do {
            --dst;
            if (dst == 0)
                dst = sz;
        } while (find(begin(chunk), end(chunk), dst) != end(chunk));

        next[chunk.back()] = next[dst];
        next[dst] = chunk.front();

        curr = next[curr];
    }

    return next;
}

auto solve1(vector<int> v)
{
    auto next = play(v, 100);

    string res;
    for (auto i = next[1]; i != 1; i = next[i])
        res.push_back(i + '0');
    return res;
}

auto solve2(vector<int> v)
{
    for (auto i = ssize(v) + 1; i <= 1000000; ++i)
        v.push_back(i);

    auto next = play(v, 10000000);

    return 1l * next[1] * next[next[1]];
}

int main()
{
    string s;
    cin >> s;

    vector<int> v;
    for (auto c : s)
        v.push_back(c - '0');

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

