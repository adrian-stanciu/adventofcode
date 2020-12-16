#include <algorithm>
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using Rules = std::unordered_map<std::string, std::string>;

auto build_all_size2_forms(std::string& image)
{
    // maximum 12 forms (4 rotations * (initial + 2 flips))
    std::unordered_set<std::string> forms;

    for (size_t i = 0; i < 4; ++i) {
        // rotate right:
        char tmp = image[0];
        image[0] = image[2];
        image[2] = image[3];
        image[3] = image[1];
        image[1] = tmp;
        forms.insert(image);

        std::string image_fr(image);
        std::string image_fu(image);

        // flip right:
        std::swap(image_fr[0], image_fr[1]);
        std::swap(image_fr[2], image_fr[3]);
        forms.insert(image_fr);

        // flig up:
        std::swap(image_fu[0], image_fu[2]);
        std::swap(image_fu[1], image_fu[3]);
        forms.insert(image_fu);
    }

    return forms;
}

auto build_all_size3_forms(std::string& image)
{
    // maximum 12 forms (4 rotations * (initial + 2 flips))
    std::unordered_set<std::string> forms;

    for (size_t i = 0; i < 4; ++i) {
        // rotate right:
        char tmp;
        tmp = image[0];
        image[0] = image[6];
        image[6] = image[8];
        image[8] = image[2];
        image[2] = tmp;
        tmp = image[1];
        image[1] = image[3];
        image[3] = image[7];
        image[7] = image[5];
        image[5] = tmp;
        forms.insert(image);

        std::string image_fr(image);
        std::string image_fu(image);

        // flip right:
        std::swap(image_fr[0], image_fr[2]);
        std::swap(image_fr[3], image_fr[5]);
        std::swap(image_fr[6], image_fr[8]);
        forms.insert(image_fr);

        // flig up:
        std::swap(image_fu[0], image_fu[6]);
        std::swap(image_fu[1], image_fu[7]);
        std::swap(image_fu[2], image_fu[8]);
        forms.insert(image_fu);
    }

    return forms;
}

auto build_all_forms(std::string& image)
{
    std::unordered_set<std::string> forms;

    if (image.size() % 2 == 0)
        forms = build_all_size2_forms(image);
    else
        forms = build_all_size3_forms(image);

    return forms;
}

void remove_slashes(std::string& rule)
{
    rule.erase(remove(rule.begin(), rule.end(), '/'), rule.end());
}

auto read_rules()
{
    Rules rules;

    std::string line;
    while (getline(std::cin, line)) {
        static const std::regex re{"(.+) => (.+)"};

        std::smatch matched;
        regex_match(line, matched, re);

        auto rule_in = matched[1].str();
        auto rule_out = matched[2].str();

        remove_slashes(rule_in);
        remove_slashes(rule_out);

        for (auto& form : build_all_forms(rule_in))
            rules[form] = rule_out;
    }

    return rules;
}

size_t count_pixels_on(const std::string& image)
{
    size_t count = 0;

    for (auto c : image)
        if (c == '#')
            ++count;

    return count;
}

std::string read_subimage(const std::string& image,
    size_t r, size_t c, size_t size)
{
    size_t image_size = sqrt(image.size());

    size_t capacity = size * size;
    std::string subimage(capacity, ' ');

    size_t k = 0;
    for (size_t i = r; i < r + size; ++i)
        for (size_t j = c; j < c + size; ++j)
            subimage[k++] = image[i * image_size + j];

    return subimage;
}

void write_subimage(std::string& image, const std::string& subimage,
    size_t r, size_t c, size_t size)
{
    size_t image_size = sqrt(image.size());

    size_t k = 0;
    for (size_t i = r; i < r + size; ++i)
        for (size_t j = c; j < c + size; ++j)
            image[i * image_size + j] = subimage[k++];
}

void enhance(std::string& image, const Rules& rules)
{
    size_t subimage_size;
    size_t new_image_size;

    if (image.size() % 2 == 0) {
        subimage_size = 2;
        new_image_size = 3;
    } else {
        subimage_size = 3;
        new_image_size = 4;
    }

    size_t subimage_count = sqrt(image.size()) / subimage_size;

    size_t new_image_capacity = pow(subimage_count * new_image_size, 2);
    std::string new_image(new_image_capacity, ' ');

    for (size_t i = 0; i < subimage_count; ++i)
        for (size_t j = 0; j < subimage_count; ++j) {
            auto subimage = read_subimage(image,
                    i * subimage_size, j * subimage_size, subimage_size);
            write_subimage(new_image, rules.find(subimage)->second,
                    i * new_image_size, j * new_image_size, new_image_size);
        }

    image = std::move(new_image);
}

size_t count_pixels_on(std::string image, const Rules& rules, size_t iters)
{
    for (size_t i = 0; i < iters; ++i)
        enhance(image, rules);

    return count_pixels_on(image);
}

int main()
{
    auto rules = read_rules();

    std::string start_rule(".#./..#/###");
    remove_slashes(start_rule);

    std::cout << count_pixels_on(start_rule, rules, 5) << "\n";
    std::cout << count_pixels_on(start_rule, rules, 18) << "\n";

    return 0;
}

