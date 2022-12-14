#include "vector2.hpp"

#include <cmath>

namespace cg_lab2
{

    Vector2 Vector2::operator-() const
    {
        return Vector2
        { 
            .x = -x, 
            .y = -y
        };
    }

    Vector2& Vector2::operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Vector2& Vector2::operator*=(double t)
    {
        x *= t;
        y *= t;

        return *this;
    }

    Vector2 Vector2::getOrthogonal() const
    {
        return Vector2(-y, x);
    }

    double Vector2::dot(const Vector2& other) const
    {
        return x * other.x + y * other.y;
    }

    double Vector2::getNorm() const
    {
        return std::sqrt(x * x + y * y);
    }

    double Vector2::getDistance(const Vector2& other) const
    {
        return (*this - other).getNorm();
    }

    double Vector2::getDet(const Vector2& other) const
    {
        return x * other.y - y * other.x;
    }

} // namespace cg_lab2

cg_lab2::Vector2 operator+(cg_lab2::Vector2 lhs, const cg_lab2::Vector2& rhs)
{
    lhs += rhs;
    return lhs;
}

cg_lab2::Vector2 operator-(cg_lab2::Vector2 lhs, const cg_lab2::Vector2& rhs)
{
    lhs -= rhs;
    return lhs;
}

cg_lab2::Vector2 operator*(double t, cg_lab2::Vector2 vec)
{
    vec *= t;
    return vec;
}

cg_lab2::Vector2 operator*(cg_lab2::Vector2 vec, double t)
{
    return t * vec;
}

std::ostream& operator<<(std::ostream& os, const cg_lab2::Vector2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}