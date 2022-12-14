#pragma once

#include <array>
#include <limits>

#include "vector2.hpp"

namespace cg_lab2
{

    class Box
    {
    public:
        enum class Side : int
        {
            Left,
            Bottom,
            Right,
            Top
        };

        struct Intersection
        {
            Side side;
            Vector2 point;
        };

        double left{ 0.0 };
        double bottom{ 0.0 };
        double right{ 0.0 };
        double top{ 0.0 };

        bool Contains(const Vector2& point) const;

        // For Fortune's algorithm
        Intersection GetFirstIntersection(const Vector2& origin, const Vector2& direction) const;

        // For diagram intersection
        int GetIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections) const;
    };

} // namespace cg_lab2