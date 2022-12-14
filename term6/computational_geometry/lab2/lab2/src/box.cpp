#include "box.hpp"

namespace cg_lab2
{

    static constexpr double g_Eps = std::numeric_limits<float>::epsilon();

    bool Box::Contains(const Vector2& point) const
    {
        return point.x >= left - g_Eps && point.x <= right + g_Eps && point.y >= bottom - g_Eps && point.y <= top + g_Eps;
    }

    Box::Intersection Box::GetFirstIntersection(const Vector2& origin, const Vector2& direction) const
    {
        Intersection intersection;
        double t{ std::numeric_limits<double>::infinity() };

        if (direction.x > 0.0)
        {
            t = (right - origin.x) / direction.x;
            intersection.side = Side::Right;
            intersection.point = origin + t * direction;
        }
        else if (direction.x < 0.0)
        {
            t = (left - origin.x) / direction.x;
            intersection.side = Side::Left;
            intersection.point = origin + t * direction;
        }
        if (direction.y > 0.0)
        {
            double newT = (top - origin.y) / direction.y;

            if (newT < t)
            {
                intersection.side = Side::Top;
                intersection.point = origin + newT * direction;
            }
        }
        else if (direction.y < 0.0)
        {
            double newT = (bottom - origin.y) / direction.y;

            if (newT < t)
            {
                intersection.side = Side::Bottom;
                intersection.point = origin + newT * direction;
            }
        }

        return intersection;
    }

    int Box::GetIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections) const
    {
        // WARNING: If the intersection is a corner, both intersections are equals
        Vector2 direction = destination - origin;
        std::array<double, 2> t;
        std::size_t i{ 0 }; // index of the current intersection

        // Left
        if (origin.x < left - g_Eps || destination.x < left - g_Eps)
        {
            t[i] = (left - origin.x) / direction.x;

            if (t[i] > g_Eps && t[i] < 1.0 - g_Eps)
            {
                intersections[i].side = Side::Left;
                intersections[i].point = origin + t[i] * direction;

                if (intersections[i].point.y >= bottom - g_Eps && intersections[i].point.y <= top + g_Eps)
                {
                    ++i;
                }
            }
        }

        // Right
        if (origin.x > right + g_Eps || destination.x > right + g_Eps)
        {
            t[i] = (right - origin.x) / direction.x;

            if (t[i] > g_Eps && t[i] < 1.0 - g_Eps)
            {
                intersections[i].side = Side::Right;
                intersections[i].point = origin + t[i] * direction;

                if (intersections[i].point.y >= bottom - g_Eps && intersections[i].point.y <= top + g_Eps)
                {
                    ++i;
                }
            }
        }

        // Bottom
        if (origin.y < bottom - g_Eps || destination.y < bottom - g_Eps)
        {
            t[i] = (bottom - origin.y) / direction.y;

            if (i < 2 && t[i] > g_Eps && t[i] < 1.0 - g_Eps)
            {
                intersections[i].side = Side::Bottom;
                intersections[i].point = origin + t[i] * direction;

                if (intersections[i].point.x >= left - g_Eps && intersections[i].point.x <= right + g_Eps)
                {
                    ++i;
                }
            }
        }

        // Top
        if (origin.y > top + g_Eps || destination.y > top + g_Eps)
        {
            t[i] = (top - origin.y) / direction.y;

            if (i < 2 && t[i] > g_Eps && t[i] < 1.0 - g_Eps)
            {
                intersections[i].side = Side::Top;
                intersections[i].point = origin + t[i] * direction;

                if (intersections[i].point.x >= left - g_Eps && intersections[i].point.x <= right + g_Eps)
                {
                    ++i;
                }
            }
        }

        // Sort the intersections from the nearest to the farthest
        if (i == 2 && t[0] > t[1])
        {
            std::swap(intersections[0], intersections[1]);
        }

        return i;
    }

} // namespace cg_lab2