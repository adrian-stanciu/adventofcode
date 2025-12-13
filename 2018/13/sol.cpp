#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using Row = std::vector<char>;
using Matrix = std::vector<Row>;

struct Cart {
    int id;
    int x;
    int y;
    int x_dir {0};
    int y_dir {0};
    int num_intersections_reached {0};
    bool crashed {false};

    Cart(int id, int x, int y, char c) : id(id), x(x), y(y)
    {
        if (c == '^')
            y_dir = -1;
        else if (c == 'v')
            y_dir = 1;
        else if (c == '<')
            x_dir = -1;
        else if (c == '>')
            x_dir = 1;
    }

    void move(const Matrix& m)
    {
        x += x_dir;
        y += y_dir;

        if (m[y][x] == '|' || m[y][x] == '-') {
            // nothing to do
        } else if (m[y][x] == '/') {
            std::swap(x_dir, y_dir);
            x_dir *= -1;
            y_dir *= -1;
        } else if (m[y][x] == '\\') {
            std::swap(x_dir, y_dir);
        } else if (m[y][x] == '+') {
            if (num_intersections_reached % 3 == 0) {
                // left
                if (y_dir == 0) {
                    x_dir *= -1;
                    y_dir *= -1;
                }
                std::swap(x_dir, y_dir);
            } else if (num_intersections_reached % 3 == 1) {
                // straight, nothing to do
            } else {
                // right
                if (x_dir == 0) {
                    x_dir *= -1;
                    y_dir *= -1;
                }
                std::swap(x_dir, y_dir);
            }
            ++num_intersections_reached;
        }
    }
};

void sort_carts(std::vector<Cart>& carts)
{
    sort(carts.begin(), carts.end(), [](const Cart& c1, const Cart& c2) {
        if (c1.y == c2.y)
            return c1.x < c2.x;
        else
            return c1.y < c2.y;
    });
}

bool check_cart_for_crash(Cart& cart, std::vector<Cart>& carts)
{
    for (auto& c : carts) {
        if (c.id == cart.id || c.crashed)
            continue;

        if ((c.x == cart.x) && (c.y == cart.y)) {
            c.crashed = true;
            cart.crashed = true;
            return true;
        }
    }

    return false;
}

void move_until_one(const Matrix& map, std::vector<Cart>& carts)
{
    std::vector<std::pair<int, int>> crash_locations;

    while (true) {
        sort_carts(carts);

        auto crash_detected = false;
        for (auto& cart : carts) {
            cart.move(map);

            if (check_cart_for_crash(cart, carts)) {
                crash_locations.emplace_back(cart.x, cart.y);
                crash_detected = true;
            }
        }

        if (crash_detected) {
            carts.erase(remove_if(carts.begin(), carts.end(), [](const Cart& cart) {
                    return cart.crashed;
                }), carts.end());
        }

        if (carts.size() == 1) {
            std::cout << crash_locations[0].first << "," << crash_locations[0].second << "\n";
            std::cout << carts[0].x << "," << carts[0].y << "\n";
            break;
        }
    }
}

int main()
{
    std::vector<Cart> carts;
    Matrix map;
    auto id = 0;
    auto y = 0;

    std::string line;
    while (getline(std::cin, line)) {
        Row r;
        auto x = 0;

        for (auto c : line) {
            if (c == '^' || c == 'v') {
                carts.emplace_back(++id, x, y, c);
                r.push_back('|');
            } else if (c == '<' || c == '>') {
                carts.emplace_back(++id, x, y, c);
                r.push_back('-');
            } else {
                r.push_back(c);
            }
            ++x;
        }

        map.push_back(move(r));
        ++y;
    }

    move_until_one(map, carts);

    return 0;
}

