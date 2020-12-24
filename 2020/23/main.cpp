#include <bits/stdc++.h>

using namespace std;

void play(list<int>& l, int iters)
{
    auto sz = l.size();
    auto curr = begin(l);

    vector<list<int>::iterator> l_pos(sz + 1);
    for (auto it = begin(l); it != end(l); ++it)
        l_pos[*it] = it;

    auto extract_chunk = [&] () {
        array<int, 3> chunk;

        auto it = next(curr);
        if (it == end(l))
            it = begin(l);

        for (auto& x : chunk) {
            x = *it;

            it = l.erase(it);
            if (it == end(l))
                it = begin(l);
        }

        return chunk;
    };

    for (auto i = 0; i < iters; ++i) {
        auto chunk = extract_chunk();
        reverse(begin(chunk), end(chunk));

        auto dst = *curr;
        do {
            --dst;
            if (dst == 0)
                dst = sz;
        } while (find(begin(chunk), end(chunk), dst) != end(chunk));
        auto dst_it = l_pos[dst];

        for (auto x : chunk)
            l_pos[x] = l.insert(next(dst_it), x);

        curr = next(curr);
        if (curr == end(l))
            curr = begin(l);
    }
}

auto solve1(list<int> l)
{
    play(l, 100);
    auto it1 = find(begin(l), end(l), 1);

    string res;
    for (auto it = next(it1); it != end(l); ++it)
        res.push_back(*it + '0');
    for (auto it = begin(l); it != it1; ++it)
        res.push_back(*it + '0');
    return res;
}

auto solve2(list<int> l)
{
    for (auto i = l.size() + 1; i <= 1000000; ++i)
        l.push_back(i);

    play(l, 10000000);
    auto it1 = find(begin(l), end(l), 1);

    auto a = *next(it1);
    auto b = *next(next(it1));
    return 1l * a * b;
}

int main()
{
    string s;
    cin >> s;

    list<int> l;
    for (auto c : s)
        l.push_back(c - '0');

    cout << solve1(l) << '\n';
    cout << solve2(l) << '\n';

    return 0;
}

