#include <bits/stdc++.h>

using namespace std;

struct Policy {
    long a;
    long b;
    char c;
};

struct Password {
    Policy pol;
    string pas;
};

int main()
{
    static const regex pw_regex{"([1-9][0-9]*)-([1-9][0-9]*) ([a-zA-Z]): (.+)"};

    vector<Password> passwords;

    string line;
    while (getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, pw_regex);

        const Policy pol{
            strtol(matched[1].str().data(), nullptr, 10),
            strtol(matched[2].str().data(), nullptr, 10),
            matched[3].str()[0]
        };
        Password pw{pol, matched[4].str()};

        passwords.push_back(move(pw));
    }

    cout << accumulate(begin(passwords), end(passwords), 0l, [] (auto acc, const auto& pw) {
        auto cnt = count_if(begin(pw.pas), end(pw.pas), [&] (auto c) {
            return c == pw.pol.c;
        });
        return acc + (cnt >= pw.pol.a && cnt <= pw.pol.b);
    }) << '\n';

    cout << accumulate(begin(passwords), end(passwords), 0l, [] (auto acc, const auto& pw) {
        return acc + ((pw.pas[pw.pol.a - 1] == pw.pol.c) ^ (pw.pas[pw.pol.b - 1] == pw.pol.c));
    }) << '\n';

    return 0;
}

