#include "fortune_algorithm.hpp"

#include <list>
#include <unordered_map>

#include "event.hpp"

namespace cg_lab2
{

    FortuneAlgorithm::FortuneAlgorithm(const std::vector<Vector2>& points)
        : m_VoronoiDiagram(points)
    {}

    void FortuneAlgorithm::Construct()
    {
        // Initialize event queue
        for (std::size_t i = 0; i < m_VoronoiDiagram.GetSiteCount(); ++i)
        {
            m_Events.push(std::make_unique<Event>(m_VoronoiDiagram.GetSite(i)));
        }

        // Process events
        while (!m_Events.isEmpty())
        {
            std::unique_ptr<Event> event = m_Events.pop();

            m_BeachLineY = event->y;

            switch (event->type)
            {
                case Event::Type::Site:
                {
                    HandleSiteEvent(event.get());
                    break;
                }
                case Event::Type::Circle:
                {
                    HandleCircleEvent(event.get());
                    break;
                }
            }
        }
    }

    void FortuneAlgorithm::HandleSiteEvent(Event* event)
    {
        VoronoiDiagram::Site* site{ event->site };

        // 1. Check if the beach line is empty
        if (m_BeachLine.isEmpty())
        {
            m_BeachLine.setRoot(m_BeachLine.createArc(site));
            return;
        }

        // 2. Look for the arc above the site
        Arc* arcToBreak{ m_BeachLine.locateArcAbove(site->point, m_BeachLineY) };
        DeleteEvent(arcToBreak);

        // 3. Replace this arc by the new arcs
        Arc* middleArc{ BreakArc(arcToBreak, site) };
        Arc* leftArc{ middleArc->prev };
        Arc* rightArc{ middleArc->next };

        // 4. Add an edge in the diagram
        AddEdge(leftArc, middleArc);
        middleArc->rightHalfEdge = middleArc->leftHalfEdge;
        rightArc->leftHalfEdge = leftArc->rightHalfEdge;

        // 5. Check circle events
        // Left triplet
        if (!m_BeachLine.isNil(leftArc->prev))
        {
            AddEvent(leftArc->prev, leftArc, middleArc);
        }

        // Right triplet
        if (!m_BeachLine.isNil(rightArc->next))
        {
            AddEvent(middleArc, rightArc, rightArc->next);
        }
    }

    void FortuneAlgorithm::HandleCircleEvent(Event* event)
    {
        Vector2 point{ event->point };
        Arc* arc{ event->arc };

        // 1. Add vertex
        VoronoiDiagram::Vertex* vertex{ m_VoronoiDiagram.CreateVertex(point) };

        // 2. Delete all the events with this arc
        Arc* leftArc{ arc->prev };
        Arc* rightArc{ arc->next };
        DeleteEvent(leftArc);
        DeleteEvent(rightArc);

        // 3. Update the beachline and the diagram
        RemoveArc(arc, vertex);

        // 4. Add new circle events
        // Left triplet
        if (!m_BeachLine.isNil(leftArc->prev))
        {
            AddEvent(leftArc->prev, leftArc, rightArc);
        }

        // Right triplet
        if (!m_BeachLine.isNil(rightArc->next))
        {
            AddEvent(leftArc, rightArc, rightArc->next);
        }
    }

    Arc* FortuneAlgorithm::BreakArc(Arc* arc, VoronoiDiagram::Site* site)
    {
        // Create the new subtree
        Arc* middleArc{ m_BeachLine.createArc(site) };

        Arc* leftArc{ m_BeachLine.createArc(arc->site) };
        leftArc->leftHalfEdge = arc->leftHalfEdge;

        Arc* rightArc{ m_BeachLine.createArc(arc->site) };
        rightArc->rightHalfEdge = arc->rightHalfEdge;

        // Insert the subtree in the beachline
        m_BeachLine.replace(arc, middleArc);
        m_BeachLine.insertBefore(middleArc, leftArc);
        m_BeachLine.insertAfter(middleArc, rightArc);

        delete arc;
        return middleArc;
    }

    void FortuneAlgorithm::RemoveArc(Arc* arc, VoronoiDiagram::Vertex* vertex)
    {
        // End edges
        SetDestination(arc->prev, arc, vertex);
        SetDestination(arc, arc->next, vertex);

        // Join the edges of the middle arc
        arc->leftHalfEdge->next = arc->rightHalfEdge;
        arc->rightHalfEdge->prev = arc->leftHalfEdge;

        // Update beachline
        m_BeachLine.remove(arc);

        // Create a new edge
        VoronoiDiagram::HalfEdge* prevHalfEdge{ arc->prev->rightHalfEdge };
        VoronoiDiagram::HalfEdge* nextHalfEdge{ arc->next->leftHalfEdge };

        AddEdge(arc->prev, arc->next);
        SetOrigin(arc->prev, arc->next, vertex);
        SetPrevHalfEdge(arc->prev->rightHalfEdge, prevHalfEdge);
        SetPrevHalfEdge(nextHalfEdge, arc->next->leftHalfEdge);

        // Delete node
        delete arc;
    }

    bool FortuneAlgorithm::IsMovingRight(const Arc* left, const Arc* right) const
    {
        return left->site->point.y < right->site->point.y;
    }

    double FortuneAlgorithm::GetInitialX(const Arc* left, const Arc* right, bool movingRight) const
    {
        return movingRight ? left->site->point.x : right->site->point.x;
    }

    void FortuneAlgorithm::AddEdge(Arc* left, Arc* right)
    {
        // Create two new half edges
        left->rightHalfEdge = m_VoronoiDiagram.CreateHalfEdge(left->site->face);
        right->leftHalfEdge = m_VoronoiDiagram.CreateHalfEdge(right->site->face);

        // Set the two half edges twins
        left->rightHalfEdge->twin = right->leftHalfEdge;
        right->leftHalfEdge->twin = left->rightHalfEdge;
    }

    void FortuneAlgorithm::SetOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
    {
        left->rightHalfEdge->destination = vertex;
        right->leftHalfEdge->origin = vertex;
    }

    void FortuneAlgorithm::SetDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
    {
        left->rightHalfEdge->origin = vertex;
        right->leftHalfEdge->destination = vertex;
    }

    void FortuneAlgorithm::SetPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next)
    {
        prev->next = next;
        next->prev = prev;
    }

    void FortuneAlgorithm::AddEvent(Arc* left, Arc* middle, Arc* right)
    {
        double y;
        const Vector2 convergencePoint{ ComputeConvergencePoint(left->site->point, middle->site->point, right->site->point, y) };

        const bool isBelow{ y <= m_BeachLineY };
        const bool leftBreakpointMovingRight{ IsMovingRight(left, middle) };
        const bool rightBreakpointMovingRight{ IsMovingRight(middle, right) };
        const double leftInitialX{ GetInitialX(left, middle, leftBreakpointMovingRight) };
        const double rightInitialX{ GetInitialX(middle, right, rightBreakpointMovingRight) };

        bool isValid =
            ((leftBreakpointMovingRight && leftInitialX < convergencePoint.x) ||
                (!leftBreakpointMovingRight && leftInitialX > convergencePoint.x)) &&
            ((rightBreakpointMovingRight && rightInitialX < convergencePoint.x) ||
                (!rightBreakpointMovingRight && rightInitialX > convergencePoint.x));

        if (isValid && isBelow)
        {
            std::unique_ptr<Event> event{ std::make_unique<Event>(static_cast<float>(y), convergencePoint, middle) };
            middle->event = event.get();
            m_Events.push(std::move(event));
        }
    }

    void FortuneAlgorithm::DeleteEvent(Arc* arc)
    {
        if (arc->event != nullptr)
        {
            m_Events.remove(arc->event->index);
            arc->event = nullptr;
        }
    }

    Vector2 FortuneAlgorithm::ComputeConvergencePoint(const Vector2& point1, const Vector2& point2, const Vector2& point3, double& y) const
    {
        const Vector2 v1{ (point1 - point2).getOrthogonal() };
        const Vector2 v2{ (point2 - point3).getOrthogonal() };
        const Vector2 delta{ 0.5 * (point3 - point1) };

        const double t{ delta.getDet(v2) / v1.getDet(v2) };
        const Vector2 center{ 0.5 * (point1 + point2) + t * v1 };
        const double r{ center.getDistance(point1) };

        y = center.y - r;

        return center;
    }

    void FortuneAlgorithm::Bound(Box box)
    {
        // Make sure the bounding box contains all the vertices
        for (const auto& vertex : m_VoronoiDiagram.GetVertices())
        {
            box.left = std::min(vertex.point.x, box.left);
            box.bottom = std::min(vertex.point.y, box.bottom);
            box.right = std::max(vertex.point.x, box.right);
            box.top = std::max(vertex.point.y, box.top);
        }

        // Retrieve all non bounded half edges from the beach line
        std::list<LinkedVertex> linkedVertices;
        std::unordered_map<std::size_t, std::array<LinkedVertex*, 8>> vertices(m_VoronoiDiagram.GetSiteCount());

        if (!m_BeachLine.isEmpty())
        {
            Arc* leftArc = m_BeachLine.getLeftmostArc();
            Arc* rightArc = leftArc->next;
            while (!m_BeachLine.isNil(rightArc))
            {
                // Bound the edge
                Vector2 direction{ (leftArc->site->point - rightArc->site->point).getOrthogonal() };
                Vector2 origin{ (leftArc->site->point + rightArc->site->point) * 0.5f };

                // Line-box intersection
                Box::Intersection intersection{ box.GetFirstIntersection(origin, direction) };

                // Create a new vertex and ends the half edges
                VoronoiDiagram::Vertex* vertex{ m_VoronoiDiagram.CreateVertex(intersection.point) };
                SetDestination(leftArc, rightArc, vertex);

                // Initialize pointers
                if (vertices.find(leftArc->site->index) == vertices.end())
                {
                    vertices[leftArc->site->index].fill(nullptr);
                }

                if (vertices.find(rightArc->site->index) == vertices.end())
                {
                    vertices[rightArc->site->index].fill(nullptr);
                }

                // Store the vertex on the boundaries
                linkedVertices.emplace_back(LinkedVertex{ nullptr, vertex, leftArc->rightHalfEdge });
                vertices[leftArc->site->index][2 * static_cast<int>(intersection.side) + 1] = &linkedVertices.back();
                linkedVertices.emplace_back(LinkedVertex{ rightArc->leftHalfEdge, vertex, nullptr });
                vertices[rightArc->site->index][2 * static_cast<int>(intersection.side)] = &linkedVertices.back();

                // Next edge
                leftArc = rightArc;
                rightArc = rightArc->next;
            }
        }

        // Add corners
        for (auto& kv : vertices)
        {
            auto& cellVertices{ kv.second };

            for (std::size_t i = 0; i < 5; ++i)
            {
                const std::size_t side{ i % 4 };
                const std::size_t nextSide{ (side + 1) % 4 };

                if (cellVertices[2 * side] == nullptr && cellVertices[2 * side + 1] != nullptr)
                {
                    std::size_t prevSide = (side + 3) % 4;
                    VoronoiDiagram::Vertex* corner = m_VoronoiDiagram.CreateCorner(box, static_cast<Box::Side>(side));
                    linkedVertices.emplace_back(LinkedVertex{ nullptr, corner, nullptr });
                    cellVertices[2 * prevSide + 1] = &linkedVertices.back();
                    cellVertices[2 * side] = &linkedVertices.back();
                }
                else if (cellVertices[2 * side] != nullptr && cellVertices[2 * side + 1] == nullptr)
                {
                    VoronoiDiagram::Vertex* corner = m_VoronoiDiagram.CreateCorner(box, static_cast<Box::Side>(nextSide));
                    linkedVertices.emplace_back(LinkedVertex{ nullptr, corner, nullptr });
                    cellVertices[2 * side + 1] = &linkedVertices.back();
                    cellVertices[2 * nextSide] = &linkedVertices.back();
                }
            }
        }

        // Join the half edges
        for (auto& kv : vertices)
        {
            std::size_t i = kv.first;
            auto& cellVertices = kv.second;
            for (std::size_t side = 0; side < 4; ++side)
            {
                if (cellVertices[2 * side] != nullptr)
                {
                    // Link vertices 
                    VoronoiDiagram::HalfEdge* halfEdge{ m_VoronoiDiagram.CreateHalfEdge(m_VoronoiDiagram.GetFace(i)) };
                    halfEdge->origin = cellVertices[2 * side]->vertex;
                    halfEdge->destination = cellVertices[2 * side + 1]->vertex;
                    cellVertices[2 * side]->nextHalfEdge = halfEdge;
                    halfEdge->prev = cellVertices[2 * side]->prevHalfEdge;

                    if (cellVertices[2 * side]->prevHalfEdge != nullptr)
                    {
                        cellVertices[2 * side]->prevHalfEdge->next = halfEdge;
                    }

                    cellVertices[2 * side + 1]->prevHalfEdge = halfEdge;
                    halfEdge->next = cellVertices[2 * side + 1]->nextHalfEdge;

                    if (cellVertices[2 * side + 1]->nextHalfEdge != nullptr)
                    {
                        cellVertices[2 * side + 1]->nextHalfEdge->prev = halfEdge;
                    }
                }
            }
        }
    }

} // namespace cg_lab2