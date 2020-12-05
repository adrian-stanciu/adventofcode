#include <bits/stdc++.h>

using namespace std;

auto bs(const string& s, const function<bool(char)>& f)
{
    auto l = 0;
    auto r = 1 << s.size();

    for (auto c : s) {
        auto m = l + (r - l) / 2;

        if (f(c))
            r = m;
        else
            l = m;
    }

    return l;
}

auto row(const string& s)
{
    return bs(s.substr(0, 7), [] (char c) {
        return c == 'F';
    });
}

auto col(const string& s)
{
    return bs(s.substr(7, 3), [] (char c) {
        return c == 'L';
    });
}

auto id(const string& s)
{
    return 8 * row(s) + col(s);
}

int main()
{
    set<int> ids;

    string line;
    while (getline(cin, line))
        ids.insert(id(line));

    auto max_id = *prev(end(ids));
    cout << max_id << '\n';

    for (auto i = 1; i < max_id; ++i)
        if (!ids.count(i) && ids.count(i - 1) && ids.count(i + 1)) {
            cout << i << '\n';
            break;
        }

    return 0;
}

