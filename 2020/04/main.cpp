#include <bits/stdc++.h>

using namespace std;

int main()
{
    const unordered_map<string, function<bool(string)>> fields{
        make_pair("byr", [] (const auto& v) {
            const auto n = strtol(v.data(), nullptr, 10);
            return n >= 1920 && n <= 2002;
        }),
        make_pair("iyr", [] (const auto& v) {
            const auto n = strtol(v.data(), nullptr, 10);
            return n >= 2010 && n <= 2020;
        }),
        make_pair("eyr", [] (const auto& v) {
            const auto n = strtol(v.data(), nullptr, 10);
            return n >= 2020 && n <= 2030;
        }),
        make_pair("hgt", [] (const auto& v) {
            char *str_end;
            const auto n = strtol(v.data(), &str_end, 10);
            const size_t n_sz = str_end - v.data();
            if (n_sz == 0 || n_sz != v.size() - 2)
                return false;
            auto unit = v.substr(n_sz, 2);
            return (unit == "cm" && n >= 150 && n <= 193) || (unit == "in" && n >= 59 && n <= 76);
        }),
        make_pair("hcl", [] (const auto& v) {
            return v.size() == 7 && v[0] == '#' && all_of(begin(v) + 1, end(v), [] (auto c) {
                return isdigit(c) || (isxdigit(c) && islower(c));
            });
        }),
        make_pair("ecl", [] (const auto& v) {
            static const unordered_set<string> colors{
                "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
            };
            return colors.count(v) > 0;
        }),
        make_pair("pid", [] (const auto& v) {
            return v.size() == 9 && all_of(begin(v), end(v), [] (auto c) {
                return isdigit(c);
            });
        }),
    };

    auto reset = [&] (auto& ht) {
        for (const auto& [k, _] : fields)
            ht[k] = false;
    };
    auto add_k = [&] (auto& ht, const auto& k) {
        if (fields.count(k) > 0)
            ht[k] = true;
    };
    auto add_kv = [&] (auto& ht, const auto& k, const auto& v) {
        if (fields.count(k) > 0)
            ht[k] |= fields.at(k)(v);
    };
    auto is_ok = [&] (const auto& ht) {
        return all_of(begin(ht), end(ht), [] (const auto& kv) {
            return kv.second;
        });
    };

    auto cnt1 = 0;
    auto cnt2 = 0;

    unordered_map<string, bool> ht1;
    reset(ht1);
    unordered_map<string, bool> ht2;
    reset(ht2);

    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            cnt1 += is_ok(ht1);
            reset(ht1);
            cnt2 += is_ok(ht2);
            reset(ht2);
        } else {
            stringstream ss{line};
            string kv;
            while (ss >> kv) {
                auto sep = kv.find_first_of(':');
                auto k = kv.substr(0, sep);
                auto v = kv.substr(sep + 1);
                add_k(ht1, k);
                add_kv(ht2, k, v);
            }
        }
    }

    cnt1 += is_ok(ht1);
    cnt2 += is_ok(ht2);

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';

    return 0;
}

