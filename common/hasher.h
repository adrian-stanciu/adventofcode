#pragma once

#include <functional>
#include <utility>

namespace {
    template <typename T>
    struct PairHasher {
        auto operator()(const std::pair<T, T>& p) const
        {
            // based on boost's hash_combine()
            size_t h = 0;
            h ^= std::hash<T>{}(p.first) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<T>{}(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}

