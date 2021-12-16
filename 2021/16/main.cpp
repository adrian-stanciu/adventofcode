#include <bits/stdc++.h>

using namespace std;

auto dec2bin(long dec)
{
    string bin;

    while (dec) {
        bin.push_back('0' + (dec & 0x1));
        dec /= 2;
    }

    while (bin.empty() || bin.size() % 4 != 0)
        bin.push_back('0');

    reverse(begin(bin), end(bin));

    return bin;
}

auto hex2bin(const string& hex)
{
    string bin;

    for (auto digit : hex)
        if (digit >= '0' && digit <= '9')
            bin.append(dec2bin(digit - '0'));
        else
            bin.append(dec2bin(10 + (digit - 'A')));

    return bin;
}

auto bin2dec(const string& bin)
{
    auto dec = 0l;

    for (auto digit : bin) {
        dec *= 2;
        dec += digit - '0';
    }

    return dec;
}

long compute(const string& bin, int& pos, int& ver_sum)
{
    ver_sum += bin2dec(bin.substr(pos, 3));
    pos += 3;

    auto type = bin2dec(bin.substr(pos, 3));
    pos += 3;

    if (type == 4) {
        string val;

        while (true) {
            auto is_last = bin[pos] == '0';
            pos += 1;

            val.append(bin.substr(pos, 4));
            pos += 4;

            if (is_last)
                break;
        }

        return bin2dec(val);
    } else {
        auto mode = bin[pos];
        pos += 1;

        vector<long> values;

        switch (mode) {
        case '0':
            {
                auto len = bin2dec(bin.substr(pos, 15));
                pos += 15;

                auto end_pos = pos + len;

                while (pos != end_pos)
                    values.push_back(compute(bin, pos, ver_sum));
            }
            break;
        case '1':
            {
                auto num = bin2dec(bin.substr(pos, 11));
                pos += 11;

                while (num--)
                    values.push_back(compute(bin, pos, ver_sum));
            }
            break;
        }

        assert(!values.empty());

        switch (type) {
        case 0:
            return accumulate(begin(values), end(values), 0l);
        case 1:
            return accumulate(begin(values), end(values), 1l, [](auto acc, auto val) {
                return acc * val;
            });
        case 2:
            return *min_element(begin(values), end(values));
        case 3:
            return *max_element(begin(values), end(values));
        case 5:
            assert(values.size() == 2);
            return (values[0] > values[1]) ? 1 : 0;
        case 6:
            assert(values.size() == 2);
            return (values[0] < values[1]) ? 1 : 0;
        case 7:
            assert(values.size() == 2);
            return (values[0] == values[1]) ? 1 : 0;
        }

        assert(false);
    }
}

auto solve(const string& hex)
{
    auto bin = hex2bin(hex);
    auto pos = 0;
    auto ver_sum = 0;

    auto res = compute(bin, pos, ver_sum);

    cout << ver_sum << '\n';
    cout << res << '\n';
}

int main()
{
    string hex;
    cin >> hex;

    solve(hex);

    return 0;
}

