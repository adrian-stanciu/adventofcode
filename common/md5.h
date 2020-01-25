#pragma once

#include <string>
#include <unordered_map>

#include <openssl/md5.h>

namespace {
    [[maybe_unused]] auto dec2hex(int d)
    {
        if (d <= 9)
            return '0' + d;
        else
            return 'a' + (d - 10);
    }

    [[maybe_unused]] auto hex2dec(int h)
    {
        if (h <= '9')
            return h - '0';
        else
            return 10 + (h - 'a');
    }

    auto hex_md5sum(const std::string& s, bool use_cache)
    {
        static std::unordered_map<std::string, std::string> cache;

        if (use_cache) {
            auto it = cache.find(s);
            if (it != cache.end())
                return it->second;
        }

        unsigned char md5sum[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(s.data()), s.size(), md5sum);

        std::string hash;
        hash.reserve(2 * MD5_DIGEST_LENGTH);

        for (auto i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            hash.push_back(dec2hex((md5sum[i] >> 4) & 0xf));
            hash.push_back(dec2hex(md5sum[i] & 0xf));
        }

        if (use_cache)
            cache[s] = hash;

        return hash;
    }
}

