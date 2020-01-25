#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

// positive double roots
auto solve_quadratic_eq_pd(long a, long b, long c)
{
    std::vector<double> roots;

    if (a == 0) {
        if (b != 0) {
            double r = -c / b;
            if (r > 0)
                roots.push_back(r);
        }
    } else {
        long delta = b * b - 4 * a * c;
        if (delta > 0) {
            double delta_sqrt = sqrt(delta);
            double r1 = (-b + delta_sqrt) / (2 * a);
            if (r1 > 0)
                roots.push_back(r1);
            double r2 = (-b - delta_sqrt) / (2 * a);
            if (r2 > 0)
                roots.push_back(r2);
        } else if (delta == 0) {
            double r = -b / (2 * a);
            if (r > 0)
                roots.push_back(r);
        }
    }

    return roots;
}

struct Particle {
    long px, py, pz;
    long vx, vy, vz;
    long ax, ay, az;

    long dist()
    {
        return abs(px) + abs(py) + abs(pz);
    }

    // compute the last time when position changes its sign on any direction
    // or 0 if position's sign on any direction is never changed
    long pva_t()
    {
        long tx = 0, ty = 0, tz = 0;

        if (px * ax < 0) {
            auto roots = solve_quadratic_eq_pd(ax, 2 * vx + ax, 2 * px);
            for (auto r : roots) {
                long t = ceil(r);
                if (t >= tx) {
                    tx = t;
                    break;
                }
            }
        }
        if (py * ay < 0) {
            auto roots = solve_quadratic_eq_pd(ay, 2 * vy + ay, 2 * py);
            for (auto r : roots) {
                long t = ceil(r);
                if (t >= ty) {
                    ty = t;
                    break;
                }
            }
        }
        if (pz * az < 0) {
            auto roots = solve_quadratic_eq_pd(az, 2 * vz + az, 2 * pz);
            for (auto r : roots) {
                long t = ceil(r);
                if (t >= tz) {
                    tz = t;
                    break;
                }
            }
        }

        return std::max(std::max(tx, ty), tz);
    }

    long p_abs(long t)
    {
        return abs(px + vx * t + ax * t * (t + 1) / 2) +
            abs(py + vy * t + ay * t * (t + 1) / 2) +
            abs(pz + vz * t + az * t * (t + 1) / 2);
    }

    // compute the last time when velocity changes its sign on any direction
    // or 0 if velocity's sign on any direction is never changed
    long va_t()
    {
        long tx = 0, ty = 0, tz = 0;

        if (vx * ax < 0)
            tx = -1 - (long)floor((double)vx / ax);
        if (vy * ay < 0)
            ty = -1 - (long)floor((double)vy / ay);
        if (vz * az < 0)
            tz = -1 - (long)floor((double)vz / az);

        return std::max(std::max(tx, ty), tz);
    }

    long v_abs(long t)
    {
        return abs(vx + ax * t) +
            abs(vy + ay * t) +
            abs(vz + az * t);
    }

    long a_abs()
    {
        return abs(ax) + abs(ay) + abs(az);
    }

    void move()
    {
        vx += ax;
        vy += ay;
        vz += az;

        px += vx;
        py += vy;
        pz += vz;
    }

    friend std::ostream& operator<<(std::ostream& out, const Particle& p)
    {
        out << "(" << p.px << "," << p.py << "," << p.pz << ") ";
        out << "(" << p.vx << "," << p.vy << "," << p.vz << ") ";
        out << "(" << p.ax << "," << p.ay << "," << p.az << ") ";

        return out;
    }
};

std::tuple<long, long, long> read_group(const std::string& group)
{
    static const std::regex re{"(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)"};

    std::smatch matched;
    regex_match(group, matched, re);

    long x = strtol(matched[1].str().data(), nullptr, 10);
    long y = strtol(matched[2].str().data(), nullptr, 10);
    long z = strtol(matched[3].str().data(), nullptr, 10);

    return std::make_tuple(x, y, z);
}

auto read_particles()
{
    static const std::regex re{"p=<(.*)>, v=<(.*)>, a=<(.*)>"};

    std::vector<Particle> particles;

    std::string line;
    while (getline(std::cin, line)) {
        Particle p;

        std::smatch matched;
        regex_match(line, matched, re);

        std::tie(p.px, p.py, p.pz) = read_group(matched[1].str());
        std::tie(p.vx, p.vy, p.vz) = read_group(matched[2].str());
        std::tie(p.ax, p.ay, p.az) = read_group(matched[3].str());

        particles.push_back(std::move(p));
    }

    return particles;
}

size_t closest_to_origin_in_long_run(std::vector<Particle> particles)
{
    long min_a = particles[0].a_abs();
    size_t min_a_idx = 0;

    for (size_t i = 1; i < particles.size(); ++i) {
        long a = particles[i].a_abs();

        if (a > min_a) {
            continue;
        } else if (a == min_a) {
            long va_t = particles[i].va_t();
            long min_va_t = particles[min_a_idx].va_t();
            long t = std::max(va_t, min_va_t);

            long v = particles[i].v_abs(t);
            long min_v = particles[min_a_idx].v_abs(t);

            if (v > min_v) {
                continue;
            } else if (v == min_v) {
                long pva_t = std::max(va_t, particles[i].pva_t());
                long min_pva_t = std::max(min_va_t, particles[min_a_idx].pva_t());
                long t = std::max(pva_t, min_pva_t);

                long p = particles[i].p_abs(t);
                long min_p = particles[min_a_idx].p_abs(t);

                if (p > min_p)
                    continue;
            }
        }

        min_a = a;
        min_a_idx = i;
    }

    return min_a_idx;
}

// non-negative integer roots
auto solve_quadratic_eq_nni(long a, long b, long c)
{
    std::vector<long> roots;

    if (a == 0) {
        if (b == 0) {
            if (c == 0)
                roots.push_back(0);
        } else {
            // check root is interger
            if (-c % b == 0) {
                long r = -c / b;
                if (r >= 0)
                    roots.push_back(r);
            }
        }
    } else {
        long delta = b * b - 4 * a * c;
        if (delta > 0) {
            long delta_sqrt = sqrt(delta);
            // check sqrt(delta) is interger
            if (delta_sqrt * delta_sqrt == delta) {
                // check root is interger
                if ((-b + delta_sqrt) % (2 * a) == 0) {
                    long r = (-b + delta_sqrt) / (2 * a);
                    if (r >= 0)
                        roots.push_back(r);
                }
                // check root is interger
                if ((-b - delta_sqrt) % (2 * a) == 0) {
                    long r = (-b - delta_sqrt) / (2 * a);
                    if (r >= 0)
                        roots.push_back(r);
                }
            }
        } else if (delta == 0) {
            // check root is interger
            if (-b % (2 * a) == 0) {
                long r = -b / (2 * a);
                if (r >= 0)
                    roots.push_back(r);
            }
        }
    }

    return roots;
}

long compute_collision_time(const Particle& p1, const Particle& p2)
{
    // times are roots of quadratic equation for x dimension
    auto roots = solve_quadratic_eq_nni(p1.ax - p2.ax,
        2 * (p1.vx - p2.vx) + (p1.ax - p2.ax),
        2 * (p1.px - p2.px));

    for (auto t : roots) {
        // test time for y dimension
        if (p1.py + p1.vy * t + p1.ay * t * (t + 1) / 2 !=
            p2.py + p2.vy * t + p2.ay * t * (t + 1) / 2)
            continue;

        // test time for z dimension
        if (p1.pz + p1.vz * t + p1.az * t * (t + 1) / 2 !=
            p2.pz + p2.vz * t + p2.az * t * (t + 1) / 2)
            continue;

        return t;
    }

    return -1;
}

size_t count_after_collisions(const std::vector<Particle>& particles)
{
    struct Collision {
        long t;
        size_t idx1;
        size_t idx2;

        Collision(long t, size_t idx1, size_t idx2)
        : t(t)
        , idx1(idx1)
        , idx2(idx2)
        {}
    };

    auto cmp_collision = [] (const Collision& c1, const Collision& c2) {
        return c1.t > c2.t;
    };

    std::priority_queue<Collision, std::vector<Collision>,
        decltype(cmp_collision)> pq(cmp_collision);

    // compute collision times and keep them sorted
    for (size_t i = 0; i < particles.size() - 1; ++i)
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto t = compute_collision_time(particles[i], particles[j]);
            if (t != -1)
                pq.emplace(t, i, j);
        }

    std::vector<long> collided(particles.size(), -1);

    while (!pq.empty()) {
        auto& collision = pq.top();

        // check not destroyed or
        // multiple collisions in the same location, at the same time
        if ((collided[collision.idx1] == -1 ||
            collided[collision.idx1] == collision.t) &&
            (collided[collision.idx2] == -1 ||
            collided[collision.idx2] == collision.t)) {
            collided[collision.idx1] = collision.t;
            collided[collision.idx2] = collision.t;
        }

        pq.pop();
    }

    return count_if(collided.begin(), collided.end(),
        [] (long collided) { return collided == -1; });
}

int main()
{
    // p0=<1,0,0>, v0=<0,0,0>, a0=<2,0,0>
    // p1=<2,0,0>, v1=<0,0,0>, a1=<-2,0,0> (closer)
    assert(closest_to_origin_in_long_run({
        {1, 0, 0, 0, 0, 0, 2, 0, 0},
        {2, 0, 0, 0, 0, 0, -2, 0, 0}
    }) == 1);
    // p0=<2,-1,0>, v0=<1,1,0>, a0=<1,1,0> (closer)
    // p1=<0,0,2>, v1=<1,0,1>, a1=<1,1,0>
    assert(closest_to_origin_in_long_run({
        {2, -1, 0, 1, 1, 0, 1, 1, 0},
        {0, 0, 2, 1, 0, 1, 1, 1, 0}
    }) == 0);
    // p0=<-1,1,0>, v0=<1,0,1>, a0=<1,0,0> (closer)
    // p1=<-1,2,0>, v1=<1,0,1>, a1=<1,0,0>
    assert(closest_to_origin_in_long_run({
        {-1, 1, 0, 1, 0, 1, 1, 0, 0},
        {-1, 2, 0, 1, 0, 1, 1, 0, 0}
    }) == 0);

    auto particles = read_particles();

    std::cout << closest_to_origin_in_long_run(particles) << "\n";
    std::cout << count_after_collisions(particles) << "\n";

    return 0;
}

