#pragma once

#include "priority_queue.hpp"
#include "voronoi_diagram.hpp"
#include "beach_line.hpp"
#include "event.hpp"

namespace cg_lab2
{

    struct Arc;

    class FortuneAlgorithm
    {
    private:
        struct LinkedVertex
        {
            VoronoiDiagram::HalfEdge* prevHalfEdge{ nullptr };
            VoronoiDiagram::Vertex* vertex{ nullptr };
            VoronoiDiagram::HalfEdge* nextHalfEdge{ nullptr };
        };

    public:
        FortuneAlgorithm(const std::vector<Vector2>& points);

    public:
        VoronoiDiagram GetVoronoiDiagram() { return std::move(m_VoronoiDiagram); }

    public:
        void Construct();
        void Bound(Box box);

    private:
        // Algorithm
        void HandleSiteEvent(Event* event);
        void HandleCircleEvent(Event* event);

        // Arcs
        Arc* BreakArc(Arc* arc, VoronoiDiagram::Site* site);
        void RemoveArc(Arc* arc, VoronoiDiagram::Vertex* vertex);

        // Breakpoint
        bool IsMovingRight(const Arc* left, const Arc* right) const;
        double GetInitialX(const Arc* left, const Arc* right, bool movingRight) const;

        // Edges
        void AddEdge(Arc* left, Arc* right);
        void SetOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
        void SetDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
        void SetPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next);

        // Events
        void AddEvent(Arc* left, Arc* middle, Arc* right);
        void DeleteEvent(Arc* arc);
        Vector2 ComputeConvergencePoint(const Vector2& point1, const Vector2& point2, const Vector2& point3, double& y) const;

    private:
        VoronoiDiagram m_VoronoiDiagram;
        BeachLine m_BeachLine;
        PriorityQueue<Event> m_Events;

        double m_BeachLineY{ 0.0 };
    };

} // namespace cg_lab2