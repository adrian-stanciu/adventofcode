#include <bits/stdc++.h>

using namespace std;

bool contains(int l1, int r1, int l2, int r2)
{
    return l1 <= l2 && r2 <= r1;
}

bool overlaps(int l1, int r1, int l2, int r2)
{
    return min(r1, r2) >= max(l1, l2);
}

int main()
{
    auto cnt1{0};
    auto cnt2{0};

    string line;
    while (getline(cin, line)) {
        int l1, r1, l2, r2;
        sscanf(line.data(), "%d-%d,%d-%d", &l1, &r1, &l2, &r2);

        cnt1 += contains(l1, r1, l2, r2) || contains(l2, r2, l1, r1);
        cnt2 += overlaps(l1, r1, l2, r2);
    }

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';

    return 0;
}

