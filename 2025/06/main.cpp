#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto compute(const vector<vector<long>>& nums, const string& ops)
{
    auto res = 0l;

    for (auto i = 0; i < ssize(ops); ++i)
        if (ops[i] == '+')
            res += accumulate(begin(nums[i]), end(nums[i]), 0l);
        else if (ops[i] == '*')
            res += accumulate(begin(nums[i]), end(nums[i]), 1l, multiplies<long>{});

    return res;
}

auto read_nums1(const vector<string>& v)
{
    vector<vector<long>> nums_mat;

    for (auto i = 0; i < ssize(v) - 1; ++i) {
        stringstream ss{v[i]};

        vector<long> nums_row;

        string s;
        while (ss >> s)
            nums_row.push_back(str2num(s));

        nums_mat.push_back(move(nums_row));
    }

    return nums_mat;
}

auto read_nums2(const vector<string>& v)
{
    vector<vector<long>> nums_mat;

    vector<long> nums_row;

    for (auto j = ssize(v[0]) - 1; j >= 0; --j) {
        auto found = false;
        auto n = 0l;

        for (auto i = 0; i < ssize(v) - 1; ++i)
            if (v[i][j] != ' ') {
                found = true;
                n *= 10;
                n += v[i][j] - '0';
            }

        if (found) {
            nums_row.push_back(n);
        } else {
            nums_mat.push_back(move(nums_row));
            nums_row.clear();
        }
    }

    if (!nums_row.empty())
        nums_mat.push_back(move(nums_row));

    reverse(begin(nums_mat), end(nums_mat));

    return nums_mat;
}

auto read_ops(const string& s)
{
    stringstream ss{s};

    string ops;

    char op;
    while (ss >> op)
        ops.push_back(op);

    return ops;
}

auto rotate_right(const vector<vector<long>>& mat)
{
    auto n = ssize(mat);
    auto m = ssize(mat[0]);

    vector<vector<long>> rot_mat(m, vector<long>(n));

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            rot_mat[j][n - i - 1] = mat[i][j];

    return rot_mat;
}

int main()
{
    vector<string> v;

    string s;
    while (getline(cin, s))
        v.push_back(move(s));

    cout << compute(rotate_right(read_nums1(v)), read_ops(v.back())) << '\n';
    cout << compute(read_nums2(v), read_ops(v.back())) << '\n';

    return 0;
}

