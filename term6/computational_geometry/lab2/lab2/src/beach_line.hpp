#pragma once

#include "vector2.hpp"
#include "voronoi_diagram.hpp"

namespace cg_lab2
{

    class Event;

    struct Arc
    {
        enum class Color
        { 
            Red,
            Black
        };

        Arc* parent{ nullptr };
        Arc* left{ nullptr };
        Arc* right{ nullptr };
        Color color{ Color::Red };

        Arc* prev{ nullptr };
        Arc* next{ nullptr };

        VoronoiDiagram::Site* site{ nullptr };
        VoronoiDiagram::HalfEdge* leftHalfEdge{ nullptr };
        VoronoiDiagram::HalfEdge* rightHalfEdge{ nullptr };
        Event* event{ nullptr };
    };

    class BeachLine
    {
    public:
        BeachLine();
        ~BeachLine();

        BeachLine(const BeachLine&) = delete;
        BeachLine& operator=(const BeachLine&) = delete;
        BeachLine(BeachLine&&) = delete;
        BeachLine& operator=(BeachLine&&) = delete;

    public:
        Arc* createArc(VoronoiDiagram::Site* site);

        bool isEmpty() const;
        bool isNil(const Arc* x) const;
        void setRoot(Arc* x);
        Arc* getLeftmostArc() const;

        Arc* locateArcAbove(const Vector2& point, double l) const;
        void insertBefore(Arc* x, Arc* y);
        void insertAfter(Arc* x, Arc* y);
        void replace(Arc* x, Arc* y);
        void remove(Arc* z);

    private:
        Arc* minimum(Arc* x) const;
        void transplant(Arc* u, Arc* v);

        void insertFixup(Arc* z);
        void removeFixup(Arc* x);

        void leftRotate(Arc* x);
        void rightRotate(Arc* y);

        double computeBreakpoint(const Vector2& point1, const Vector2& point2, double l) const;

        void free(Arc* x);

    private:
        Arc* mNil;
        Arc* mRoot;
    };

} // namespace cg_lab2