//
// Created by Bartomiej Tempka on 28/05/2023.
//

#ifndef N_BODY_SIM_GEOMETRY_H
#define N_BODY_SIM_GEOMETRY_H
#include <iostream>
static constexpr float inf = std::numeric_limits<float>::infinity();
struct point{

    double x;
    double y;
    friend std::ostream& operator<<(std::ostream& out, const point& p)
    {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }

    point operator * (double const & d) {
        return {x * d, y * d};
    }

    point & operator += (point const & p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    point & operator /= (double const & d) {
        x /= d;
        y /= d;
        return *this;
    }
};

struct box{
    point bottom_left{inf, inf};
    point top_right{-inf, -inf};


    box & operator |= (gravity_object const & p) {
        bottom_left.x = std::min(bottom_left.x, p.x);
        bottom_left.y = std::min(bottom_left.y, p.y);
        top_right.x = std::max(top_right.x, p.x);
        top_right.y = std::max(top_right.y, p.y);
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const box& b)
    {
        os << "[" << b.bottom_left << ", " << b.top_right << "]";
        return os;
    }
};

static box bbox(std::vector<gravity_object>::iterator begin, std::vector<gravity_object>::iterator end)
{
    box b;
    for (auto it = begin; it != end; ++it)
        b |= *it;
    return b;
}
static point middle(point const & p1, point const & p2)
{
    return { (p1.x + p2.x) / 2.f, (p1.y + p2.y) / 2.f };
}

#endif //N_BODY_SIM_GEOMETRY_H
