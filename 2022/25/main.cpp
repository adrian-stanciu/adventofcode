#include <bits/stdc++.h>

using namespace std;

int to_dec_digit(char snafu_digit)
{
    switch (snafu_digit) {
    case '-':
        return -1;
    case '=':
        return -2;
    case '0':
    case '1':
    case '2':
        return snafu_digit - '0';
    }
    assert(false);
}

auto to_dec(const string& snafu)
{
    auto dec = 0l;
    for (auto snafu_digit : snafu)
        dec = 5 * dec + to_dec_digit(snafu_digit);
    return dec;
}

char to_snafu_digit(int dec_digit)
{
    switch (dec_digit) {
    case 0:
    case 1:
    case 2:
        return dec_digit + '0';
    case 3:
        return '=';
    case 4:
        return '-';
    }
    assert(false);
}

auto to_snafu(long dec)
{
    string snafu;

    while (dec) {
        auto dec_digit = dec % 5;
        dec /= 5;

        auto snafu_digit = to_snafu_digit(dec_digit);
        snafu.push_back(snafu_digit);

        if (snafu_digit == '-' || snafu_digit == '=')
            ++dec;
    }

    reverse(begin(snafu), end(snafu));

    return snafu;
}

int main()
{
    auto sum = 0l;

    string snafu;
    while (getline(cin, snafu))
        sum += to_dec(snafu);

    cout << to_snafu(sum) << '\n';

    return 0;
}

