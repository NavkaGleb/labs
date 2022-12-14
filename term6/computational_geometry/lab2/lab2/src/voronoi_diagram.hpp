#pragma once

#include <vector>
#include <list>

#include "box.hpp"

namespace cg_lab2
{

    class Box;

    class VoronoiDiagram
    {
    public:
        friend class FortuneAlgorithm;

    public:
        struct Site;
        struct HalfEdge;

        struct Face
        {
            Site* site{ nullptr };
            HalfEdge* outerComponent{ nullptr };
        };

        struct Site
        {
            std::size_t index{ 0 };
            Vector2 point;
            Face* face{ nullptr };
        };

        struct Vertex
        {
            Vector2 point;

        private:
            friend VoronoiDiagram;
            std::list<Vertex>::iterator it;
        };

        struct HalfEdge
        {
            Vertex* origin{ nullptr };
            Vertex* destination{ nullptr };
            HalfEdge* twin{ nullptr };
            Face* incidentFace{ nullptr };
            HalfEdge* prev{ nullptr };
            HalfEdge* next{ nullptr };

        private:
            friend VoronoiDiagram;
            std::list<HalfEdge>::iterator it;
        };

    public:
        VoronoiDiagram(const std::vector<Vector2>& points);

        VoronoiDiagram(const VoronoiDiagram&) = delete;
        VoronoiDiagram& operator=(const VoronoiDiagram&) = delete;

        VoronoiDiagram(VoronoiDiagram&&) = default;
        VoronoiDiagram& operator=(VoronoiDiagram&&) = default;

    public:
        Site* GetSite(std::size_t i) { return &m_Sites[i]; };
        std::size_t GetSiteCount() const { return m_Sites.size(); };
        Face* GetFace(std::size_t i) { return &m_Faces[i]; }

        const std::list<Vertex>& GetVertices() const { return m_Vertices; }
        const std::list<HalfEdge>& GetHalfEdges() const { return m_HalfEdges; }

    public:
        bool intersect(Box box);

    private:
        Vertex* CreateVertex(Vector2 point);
        Vertex* CreateCorner(const Box& box, Box::Side side);
        HalfEdge* CreateHalfEdge(Face* face);

        // Intersection with a box
        void Link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide);
        void RemoveVertex(Vertex* vertex);
        void RemoveHalfEdge(HalfEdge* halfEdge);

    private:
        std::vector<Site> m_Sites;
        std::vector<Face> m_Faces;
        std::list<Vertex> m_Vertices;
        std::list<HalfEdge> m_HalfEdges;
    };

} // namespace cg_lab2