#include <bits/stdc++.h>

using namespace std;

auto find_error(const vector<long>& v)
{
    auto is_valid = [&] (auto l, auto r) {
        for (auto i = l; i < r; ++i)
            for (auto j = i + 1; j < r; ++j)
                if (v[i] + v[j] == v[r])
                    return true;
        return false;
    };

    for (size_t l = 0, r = 25; r < v.size(); ++l, ++r)
        if (!is_valid(l, r))
            return v[r];

    return -1l; // not found
}

auto find_weakness(const vector<long>& v, long err)
{
    for (size_t i = 0; i < v.size(); ++i) {
        auto s = 0l;
        auto lo = numeric_limits<long>::max();
        auto hi = numeric_limits<long>::min();

        for (size_t j = i; j < v.size(); ++j) {
            s += v[j];
            lo = min(lo, v[j]);
            hi = max(hi, v[j]);

            if (s == err)
                return lo + hi;
        }
    }

    return -1l; // not found
}

int main()
{
    vector<long> v;

    long x;
    while (cin >> x)
        v.push_back(x);

    auto err = find_error(v);
    cout << err << '\n';
    cout << find_weakness(v, err) << '\n';

    return 0;
}

