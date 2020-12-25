#include <bits/stdc++.h>

using namespace std;

const auto Mod = 20201227;

auto compute_loop_size(int subject, int exp_pub_key)
{
    for (auto i = 1, pub_key = 1;; ++i) {
        pub_key = (1l * pub_key * subject) % Mod;
        if (pub_key == exp_pub_key)
            return i;
    }
}

auto compute_enc_key(int subject, int loop_size)
{
    auto enc_key = 1;

    while (loop_size) {
        if (loop_size & 0x1) {
            enc_key = (1l * enc_key * subject) % Mod;
            --loop_size;
        }

        subject = (1l * subject * subject) % Mod;
        loop_size /= 2;
    }

    return enc_key;
}

int main()
{
    int card_pub_key, door_pub_key;
    cin >> card_pub_key >> door_pub_key;

    auto enc_key = compute_enc_key(door_pub_key, compute_loop_size(7, card_pub_key));
    assert(enc_key == compute_enc_key(card_pub_key, compute_loop_size(7, door_pub_key)));
    cout << enc_key << '\n';

    return 0;
}

