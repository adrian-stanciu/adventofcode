#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Program {
    std::string name;
    long weight;
    std::vector<std::string> children;
    mutable long total_weight;

    Program(std::string&& name, long weight, std::vector<std::string>&& children)
    : name(std::move(name))
    , weight(weight)
    , children(std::move(children))
    , total_weight(0)
    {}

    friend std::ostream& operator<<(std::ostream& out, const Program& p)
    {
        out << p.name << " " << p.weight;
        for (auto& child : p.children)
            out << " " << child;

        return out;
    }
};

auto read_programs()
{
    std::unordered_map<std::string, Program> programs;

    std::string line;
    while (getline(std::cin, line)) {
        static const std::regex leaf_re{R"(([a-z]+) \(([1-9][0-9]*)\))"};
        static const std::regex inter_re{R"(([a-z]+) \(([1-9][0-9]*)\) -> (.+))"};

        std::string name;
        long weight;
        std::vector<std::string> children;

        std::smatch matched;
        if (regex_match(line, matched, leaf_re)) {
            name = matched[1].str();
            weight = strtol(matched[2].str().data(), nullptr, 10);
        } else if (regex_match(line, matched, inter_re)) {
            name = matched[1].str();
            weight = strtol(matched[2].str().data(), nullptr, 10);

            std::stringstream ss(matched[3].str());
            std::string child;
            while (ss >> child) {
                if (child.back() == ',')
                    child.pop_back();
                children.push_back(std::move(child));
            }
        } else
            continue;

        std::string key(name);
        programs.emplace(std::piecewise_construct,
                forward_as_tuple(std::move(key)),
                forward_as_tuple(std::move(name), weight, std::move(children)));
    }

    return programs;
}

std::string root_program(const std::unordered_map<std::string, Program>& programs)
{
    std::unordered_set<std::string> unique_programs;

    for (auto& p : programs)
        unique_programs.insert(p.first);

    for (auto& p : programs)
        for (auto& child : p.second.children) {
            auto it = unique_programs.find(child);
            if (it != unique_programs.end())
                unique_programs.erase(it);
        }

    if (unique_programs.size() == 1)
        return *unique_programs.begin();
    else
        return "";
}

const Program& get_program(const std::string& root,
    const std::unordered_map<std::string, Program>& programs)
{
    auto it = programs.find(root);
    return it->second;
}

long compute_total_weight(const std::string& root,
    std::unordered_map<std::string, Program>& programs)
{
    auto& p = get_program(root, programs);

    auto children_weight = 0L;
    for (auto& child : p.children)
        children_weight += compute_total_weight(child, programs);

    p.total_weight = p.weight + children_weight;

    return p.total_weight;
}

long get_maj_weight(const std::vector<std::string>& children,
    const std::unordered_map<std::string, Program>& programs)
{
    if (children.size() == 0)
        return 0;

    auto& child = get_program(children[0], programs);
    auto maj_weight = child.total_weight;
    size_t maj_count = 1;

    for (size_t i = 1; i < children.size(); ++i) {
        auto& child = get_program(children[i], programs);

        if (child.total_weight == maj_weight) {
            ++maj_count;
        } else {
            --maj_count;
            if (maj_count == 0) {
                maj_weight = child.total_weight;
                maj_count = 1;
            }
        }
    }

    return maj_weight;
}

bool children_have_same_total_weight(const Program& parent,
    const std::unordered_map<std::string, Program>& programs)
{
    if (parent.children.size() == 0)
        return true;

    auto& child = get_program(parent.children[0], programs);
    auto needed_weight = child.total_weight;

    for (size_t i = 1; i < parent.children.size(); ++i) {
        auto& child = get_program(parent.children[i], programs);
        if (child.total_weight != needed_weight)
            return false;
    }

    return true;
}

long find_correct_weight(const Program& p,
    const std::unordered_map<std::string, Program>& programs, long needed_weight)
{
    // one child has a total weight different from than of other children
    // because a node cannot hide wrong weights in his subtree
    // (it is guaranteed that only one node has wrong weight)

    if (p.children.size() > 2) {
        auto maj_weight = get_maj_weight(p.children, programs);

        for (const auto& c : p.children) {
            auto& child = get_program(c, programs);
            if (child.total_weight != maj_weight)
                return find_correct_weight(child, programs, maj_weight);
        }
    } else if (p.children.size() == 2) {
        auto& child1 = get_program(p.children[0], programs);
        auto& child2 = get_program(p.children[1], programs);

        if (child1.total_weight != child2.total_weight) {
            // which child has wrong total weight? ... check both
            if (!children_have_same_total_weight(child1, programs))
                return find_correct_weight(child1, programs,
                        child2.total_weight);

            if (!children_have_same_total_weight(child2, programs))
                return find_correct_weight(child2, programs,
                        child1.total_weight);

            auto child_correct_total_weight = (needed_weight - p.weight) / 2;

            if (child1.total_weight != child_correct_total_weight)
                return child1.weight + (child_correct_total_weight - child1.total_weight);
            else if (child2.total_weight != child_correct_total_weight)
                return child2.weight + (child_correct_total_weight - child2.total_weight);
        }
    } else if (p.children.size() == 1) {
        assert(false); // ambiguous: who has a wrong weight? parent or child?
    }

    // no children or all children have the same total weight
    return p.weight + (needed_weight - p.total_weight);
}

int main()
{
    auto programs = read_programs();

    auto root = root_program(programs);
    std::cout << root << "\n";

    compute_total_weight(root, programs);

    auto& p = get_program(root, programs);
    std::cout << find_correct_weight(p, programs, p.total_weight) << "\n";

    return 0;
}

