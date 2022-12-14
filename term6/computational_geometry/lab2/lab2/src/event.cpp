#include "event.hpp"

namespace cg_lab2
{

    Event::Event(VoronoiDiagram::Site* site)
        : type(Type::Site)
        , y(site->point.y)
        , site(site)
    {}

    Event::Event(float y, Vector2 point, Arc* arc)
        : type(Type::Circle)
        , y(y)
        , point(point)
        , arc(arc)
    {}

} // namespace cg_lab2

bool operator<(const cg_lab2::Event& lhs, const cg_lab2::Event& rhs)
{
    return lhs.y < rhs.y;
}