#include <bits/stdc++.h>

using namespace std;

int main()
{
    auto is = [](const auto& v, char first, char last) {
        const auto n = ssize(v);
        const auto m = ssize(v[0]);

        return count(begin(v[0]), end(v[0]), first) == m &&
            count(begin(v[n - 1]), end(v[n - 1]), last) == m;
    };
    auto is_key = [&](const auto& v) {
        return is(v, '.', '#');
    };
    auto is_lock = [&](const auto& v) {
        return is(v, '#', '.');
    };

    auto compute_heights = [](const auto& v) {
        const auto n = ssize(v);
        const auto m = ssize(v[0]);

        vector<int> heights;
        for (auto j = 0; j < m; ++j)
            for (auto i = 0; i < n; ++i)
                if (v[i][j] != '#') {
                    heights.push_back(i);
                    break;
                }
        return heights;
    };

    vector<vector<int>> keys;
    vector<vector<int>> locks;
    optional<int> max_height;

    string s;
    while (getline(cin, s)) {
        vector<string> v;
        v.push_back(move(s));
        while (getline(cin, s)) {
            if (s.empty())
                break;
            v.push_back(move(s));
        }

        if (is_key(v)) {
            reverse(begin(v), end(v));
            keys.push_back(compute_heights(v));
        } else if (is_lock(v)) {
            locks.push_back(compute_heights(v));
        } else {
            assert(false);
        }

        if (!max_height)
            max_height = ssize(v);
        else
            assert(ssize(v) == *max_height);
    }

    auto is_valid = [&](const auto& key, const auto& lock) {
        for (auto i = 0; i < ssize(key); ++i)
            if (key[i] + lock[i] > *max_height)
                return false;
       return true;
    };

    auto cnt = 0;
    for (const auto& key : keys)
        for (const auto& lock : locks)
            cnt += is_valid(key, lock);
    cout << cnt << '\n';

    return 0;
}

