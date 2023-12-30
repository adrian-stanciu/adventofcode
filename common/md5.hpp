#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <mbedtls/md5.h>

namespace {
    [[maybe_unused]] inline auto dec2hex(int d)
    {
        if (d <= 9)
            return '0' + d;
        else
            return 'a' + (d - 10);
    }

    [[maybe_unused]] inline auto hex2dec(int h)
    {
        if (h <= '9')
            return h - '0';
        else
            return 10 + (h - 'a');
    }

    inline auto hex_md5sum(const std::string& s, bool use_cache)
    {
        static constexpr auto MD5SumLen{16};

        static std::unordered_map<std::string, std::string> cache;

        if (use_cache) {
            auto it = cache.find(s);
            if (it != cache.end())
                return it->second;
        }

        std::array<unsigned char, MD5SumLen> md5sum;
        mbedtls_md5_ret(reinterpret_cast<const unsigned char*>(s.data()), s.size(), md5sum.data());

        std::string hash;
        hash.reserve(2 * MD5SumLen);

        for (auto byte : md5sum) {
            hash.push_back(dec2hex((byte >> 4) & 0xf));
            hash.push_back(dec2hex(byte & 0xf));
        }

        if (use_cache)
            cache[s] = hash;

        return hash;
    }
}

