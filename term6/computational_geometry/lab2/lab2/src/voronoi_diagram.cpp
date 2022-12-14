#include "voronoi_diagram.hpp"

#include <unordered_set>

namespace cg_lab2
{

    VoronoiDiagram::VoronoiDiagram(const std::vector<Vector2>& points)
    {
        m_Sites.reserve(points.size());
        m_Faces.reserve(points.size());

        for (std::size_t i = 0; i < points.size(); ++i)
        {
            m_Sites.push_back(VoronoiDiagram::Site{ i, points[i], nullptr });
            m_Faces.push_back(VoronoiDiagram::Face{ &m_Sites.back(), nullptr });
            m_Sites.back().face = &m_Faces.back();
        }
    }

    bool VoronoiDiagram::intersect(Box box)
    {
        bool error{ false };
        std::unordered_set<HalfEdge*> processedHalfEdges;
        std::unordered_set<Vertex*> verticesToRemove;

        for (const Site& site : m_Sites)
        {
            HalfEdge* halfEdge = site.face->outerComponent;
            bool inside{ box.Contains(halfEdge->origin->point) };
            bool outerComponentDirty{ !inside };

            HalfEdge* incomingHalfEdge{ nullptr }; // First half edge coming in the box
            HalfEdge* outgoingHalfEdge{ nullptr }; // Last half edge going out the box

            Box::Side incomingSide;
            Box::Side outgoingSide;

            do
            {
                std::array<Box::Intersection, 2> intersections;
                int nbIntersections{ box.GetIntersections(halfEdge->origin->point, halfEdge->destination->point, intersections) };
                bool nextInside{ box.Contains(halfEdge->destination->point) };
                HalfEdge* nextHalfEdge = halfEdge->next;
                // The two points are outside the box 
                if (!inside && !nextInside)
                {
                    // The edge is outside the box
                    if (nbIntersections == 0)
                    {
                        verticesToRemove.emplace(halfEdge->origin);
                        RemoveHalfEdge(halfEdge);
                    }
                    // The edge crosses twice the frontiers of the box
                    else if (nbIntersections == 2)
                    {
                        verticesToRemove.emplace(halfEdge->origin);
                        if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                        {
                            halfEdge->origin = halfEdge->twin->destination;
                            halfEdge->destination = halfEdge->twin->origin;
                        }
                        else
                        {
                            halfEdge->origin = CreateVertex(intersections[0].point);
                            halfEdge->destination = CreateVertex(intersections[1].point);
                        }
                        if (outgoingHalfEdge != nullptr)
                            Link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
                        if (incomingHalfEdge == nullptr)
                        {
                            incomingHalfEdge = halfEdge;
                            incomingSide = intersections[0].side;
                        }
                        outgoingHalfEdge = halfEdge;
                        outgoingSide = intersections[1].side;
                        processedHalfEdges.emplace(halfEdge);
                    }
                    else
                        error = true;
                }
                // The edge is going outside the box
                else if (inside && !nextInside)
                {
                    if (nbIntersections == 1)
                    {
                        if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                            halfEdge->destination = halfEdge->twin->origin;
                        else
                            halfEdge->destination = CreateVertex(intersections[0].point);
                        outgoingHalfEdge = halfEdge;
                        outgoingSide = intersections[0].side;
                        processedHalfEdges.emplace(halfEdge);
                    }
                    else
                    {
                        error = true;
                    }
                }
                // The edge is coming inside the box
                else if (!inside && nextInside)
                {
                    if (nbIntersections == 1)
                    {
                        verticesToRemove.emplace(halfEdge->origin);
                        if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                            halfEdge->origin = halfEdge->twin->destination;
                        else
                            halfEdge->origin = CreateVertex(intersections[0].point);
                        if (outgoingHalfEdge != nullptr)
                            Link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
                        if (incomingHalfEdge == nullptr)
                        {
                            incomingHalfEdge = halfEdge;
                            incomingSide = intersections[0].side;
                        }
                        processedHalfEdges.emplace(halfEdge);
                    }
                    else
                        error = true;
                }
                halfEdge = nextHalfEdge;
                // Update inside
                inside = nextInside;
            } while (halfEdge != site.face->outerComponent);

            // Link the last and the first half edges inside the box
            if (outerComponentDirty && incomingHalfEdge != nullptr)
            {
                Link(box, outgoingHalfEdge, outgoingSide, incomingHalfEdge, incomingSide);
            }

            // Set outer component
            if (outerComponentDirty)
            {
                site.face->outerComponent = incomingHalfEdge;
            }
        }

        // Remove vertices
        for (auto& vertex : verticesToRemove)
        {
            RemoveVertex(vertex);
        }

        return !error;
    }

    VoronoiDiagram::Vertex* VoronoiDiagram::CreateVertex(Vector2 point)
    {
        m_Vertices.emplace_back();
        m_Vertices.back().point = point;
        m_Vertices.back().it = std::prev(m_Vertices.end());

        return &m_Vertices.back();
    }

    VoronoiDiagram::Vertex* VoronoiDiagram::CreateCorner(const Box& box, Box::Side side)
    {
        switch (side)
        {
            case Box::Side::Left:
            {
                return CreateVertex(Vector2(box.left, box.top));
            }
            case Box::Side::Bottom:
            {
                return CreateVertex(Vector2(box.left, box.bottom));
            }
            case Box::Side::Right:
            {
                return CreateVertex(Vector2(box.right, box.bottom));
            }
            case Box::Side::Top:
            {
                return CreateVertex(Vector2(box.right, box.top));
            }
            default:
            {
                break;
            }
        }

        return nullptr;
    }

    VoronoiDiagram::HalfEdge* VoronoiDiagram::CreateHalfEdge(Face* face)
    {
        m_HalfEdges.emplace_back();
        m_HalfEdges.back().incidentFace = face;
        m_HalfEdges.back().it = std::prev(m_HalfEdges.end());

        if (face->outerComponent == nullptr)
        {
            face->outerComponent = &m_HalfEdges.back();
        }

        return &m_HalfEdges.back();
    }

    void VoronoiDiagram::Link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide)
    {
        HalfEdge* halfEdge{ start };
        int side = static_cast<int>(startSide);

        while (side != static_cast<int>(endSide))
        {
            side = (side + 1) % 4;
            halfEdge->next = CreateHalfEdge(start->incidentFace);
            halfEdge->next->prev = halfEdge;
            halfEdge->next->origin = halfEdge->destination;
            halfEdge->next->destination = CreateCorner(box, static_cast<Box::Side>(side));
            halfEdge = halfEdge->next;
        }

        halfEdge->next = CreateHalfEdge(start->incidentFace);
        halfEdge->next->prev = halfEdge;
        end->prev = halfEdge->next;
        halfEdge->next->next = end;
        halfEdge->next->origin = halfEdge->destination;
        halfEdge->next->destination = end->origin;
    }

    void VoronoiDiagram::RemoveVertex(Vertex* vertex)
    {
        m_Vertices.erase(vertex->it);
    }

    void VoronoiDiagram::RemoveHalfEdge(HalfEdge* halfEdge)
    {
        m_HalfEdges.erase(halfEdge->it);
    }

} // namespace cg_lab2