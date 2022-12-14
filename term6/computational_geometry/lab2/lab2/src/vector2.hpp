#pragma once

#include <ostream>

namespace cg_lab2
{

    struct Vector2
    {
        double x{ 0.0 };
        double y{ 0.0 };

        Vector2 operator-() const;
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator*=(double t);

        Vector2 getOrthogonal() const;
        double dot(const Vector2& other) const;
        double getNorm() const;
        double getDistance(const Vector2& other) const;
        double getDet(const Vector2& other) const;
    };

} // namespace cg_lab2

// Binary operators
cg_lab2::Vector2 operator+(cg_lab2::Vector2 lhs, const cg_lab2::Vector2& rhs);
cg_lab2::Vector2 operator-(cg_lab2::Vector2 lhs, const cg_lab2::Vector2& rhs);
cg_lab2::Vector2 operator*(double t, cg_lab2::Vector2 vec);
cg_lab2::Vector2 operator*(cg_lab2::Vector2 vec, double t);

std::ostream& operator<<(std::ostream& os, const cg_lab2::Vector2& vec);