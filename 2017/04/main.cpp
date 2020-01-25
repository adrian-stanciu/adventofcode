#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

using Passphrase = std::vector<std::string>;

std::vector<Passphrase> read_passphrases()
{
    std::vector<Passphrase> passphrases;

    std::string line;
    while (getline(std::cin, line)) {
        std::stringstream ss(line);
        std::vector<std::string> words;
        copy(std::istream_iterator<std::string>(ss),
            std::istream_iterator<std::string>(), back_inserter(words));

        Passphrase pp;
        for (auto& word : words)
            pp.push_back(std::move(word));

        passphrases.push_back(std::move(pp));
    }

    return passphrases;
}

unsigned long count_no_repeat_word_passphrases(const std::vector<Passphrase>& passphrases)
{
    unsigned long count = 0;

    for (auto& pp : passphrases) {
        bool ok = true;
        std::unordered_set<std::string> unique_words;
        for (auto& word : pp) {
            if (unique_words.find(word) == unique_words.end()) {
                unique_words.insert(word);
            } else {
                ok = false;
                break;
            }
        }

        if (ok)
            ++count;
    }

    return count;
}

unsigned long count_no_repeat_anagram_passphrases(const std::vector<Passphrase>& passphrases)
{
    unsigned long count = 0;

    for (auto& pp : passphrases) {
        bool ok = true;
        std::unordered_set<std::string> unique_words;
        for (auto& word : pp) {
            std::string sorted_word(word);
            sort(sorted_word.begin(), sorted_word.end());

            if (unique_words.find(sorted_word) == unique_words.end()) {
                unique_words.insert(sorted_word);
            } else {
                ok = false;
                break;
            }
        }

        if (ok)
            ++count;
    }

    return count;
}

int main()
{
    auto passphrases = read_passphrases();

    std::cout << count_no_repeat_word_passphrases(passphrases) << "\n";
    std::cout << count_no_repeat_anagram_passphrases(passphrases) << "\n";

    return 0;
}

