#pragma once

#include "vector2.hpp"
#include "voronoi_diagram.hpp"

namespace cg_lab2
{

    struct Arc;

    class Event
    {
    public:
        enum class Type
        {
            Site,
            Circle
        };

        Event() = default;

        // Site event
        Event(VoronoiDiagram::Site* site);

        // Circle event
        Event(float y, Vector2 point, Arc* arc);

        const Type type;

        float y{ 0.0f };
        int index{ -1 };

        // Site event
        VoronoiDiagram::Site* site{ nullptr };

        // Circle event
        Vector2 point;
        Arc* arc{ nullptr };
    };

} // namespace cg_lab2

bool operator<(const cg_lab2::Event& lhs, const cg_lab2::Event& rhs);