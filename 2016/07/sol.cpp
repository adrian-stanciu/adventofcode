#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Chunk {
    int off;
    int len;

    Chunk(int off, int len)
    : off(off), len(len)
    {}
};

auto parse(const std::string& addr)
{
    std::vector<Chunk> outer;
    std::vector<Chunk> inner;

    size_t pos = 0;

    while (true) {
        auto lb = addr.find_first_of('[', pos);
        if (lb != std::string::npos) {
            outer.emplace_back(pos, lb - pos);

            auto rb = addr.find_first_of(']', lb + 1);
            inner.emplace_back(lb + 1, rb - lb - 1);

            pos = rb + 1;
        } else {
            outer.emplace_back(pos, addr.size() - pos);
            break;
        }
    }

    return make_pair(outer, inner);
}

bool has_abba(const std::string& s, const Chunk& chunk)
{
    auto is_abba = [&] (int off) {
        return s[off] != s[off + 1] && s[off] == s[off + 3] && s[off + 1] == s[off + 2];
    };

    for (auto i = chunk.off; i < chunk.off + chunk.len - 3; ++i)
        if (is_abba(i))
            return true;

    return false;
}

bool is_ipv7_tls(const std::string& addr)
{
    auto [outer, inner] = parse(addr);

    auto has_abba = [&] (const auto& chunk) {
        return ::has_abba(addr, chunk);
    };

    return none_of(inner.begin(), inner.end(), has_abba) &&
        any_of(outer.begin(), outer.end(), has_abba);
}

auto get_aba_set(const std::string& addr, const Chunk& chunk)
{
    auto is_aba = [&] (int off) {
        return addr[off] != addr[off + 1] && addr[off] == addr[off + 2];
    };

    std::unordered_set<std::string> aba_set;

    for (auto i = chunk.off; i < chunk.off + chunk.len - 2; ++i)
        if (is_aba(i))
            aba_set.emplace(addr.substr(i, 3));

    return aba_set;
}

auto get_aba_set(const std::string& addr, const std::vector<Chunk>& chunks)
{
    std::unordered_set<std::string> aba_set;

    for (const auto& chunk : chunks) {
        auto aba_set_in_chunk = get_aba_set(addr, chunk);
        aba_set.insert(aba_set_in_chunk.begin(), aba_set_in_chunk.end());
    }

    return aba_set;
}

bool is_ipv7_ssl(const std::string& addr)
{
    auto [outer, inner] = parse(addr);

    auto outer_aba_set = get_aba_set(addr, outer);
    auto inner_aba_set = get_aba_set(addr, inner);

    for (const auto& aba : outer_aba_set) {
        const std::string bab{aba[1], aba[0], aba[1]};

        if (inner_aba_set.count(bab) > 0)
            return true;
    }

    return false;
}

auto count_ipv7(const std::vector<std::string>& addrs,
    const std::function<bool(const std::string)>& is_ipv7_fn)
{
    auto cnt = 0;

    for (const auto& addr : addrs)
        if (is_ipv7_fn(addr))
            ++cnt;

    return cnt;
}

int main()
{
    std::vector<std::string> addrs;

    std::string line;
    while (getline(std::cin, line))
        addrs.push_back(std::move(line));

    std::cout << count_ipv7(addrs, is_ipv7_tls) << "\n";
    std::cout << count_ipv7(addrs, is_ipv7_ssl) << "\n";

    return 0;
}

