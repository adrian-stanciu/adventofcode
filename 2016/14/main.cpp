#include <iostream>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "md5.hpp"

std::optional<char> find_first_triplet(const std::string& hex)
{
    for (auto i = 0U; i < hex.size() - 2; ++i)
        if (hex[i] == hex[i + 1] && hex[i] == hex[i + 2])
            return hex[i];

    return {};
}

auto find_index(const std::string& salt, int num_keys,
    const std::function<std::string(std::string, bool)>& hash_fn)
{
    auto idx = 0L;

    while (true) {
        auto hex = hash_fn(salt + std::to_string(idx), true);

        auto c = find_first_triplet(hex);
        if (c.has_value()) {
            const std::string target(5, c.value());

            for (auto i = idx + 1; i <= idx + 1000; ++i)
                if (hash_fn(salt + std::to_string(i), true).find(target) != std::string::npos) {
                    --num_keys;
                    break;
                }
        }

        if (num_keys == 0)
            return idx;

        ++idx;
    }

    return idx;
}

template <int N>
std::string hex_md5sum_with_key_stretching(const std::string& s, bool use_cache)
{
    static std::unordered_map<std::string, std::string> cache;

    if (use_cache) {
        auto it = cache.find(s);
        if (it != cache.end())
            return it->second;
    }

    auto hash = hex_md5sum(s, true);

    for (auto i = 1; i <= N; ++i)
        hash = hex_md5sum(hash, false);

    if (use_cache)
        cache[s] = hash;

    return hash;
}

int main()
{
    std::string id;
    std::cin >> id;

    std::cout << find_index(id, 64, hex_md5sum) << "\n";
    std::cout << find_index(id, 64, hex_md5sum_with_key_stretching<2016>) << "\n";

    return 0;
}

