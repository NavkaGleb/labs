#include "beach_line.hpp"

#include <limits>
#include <cmath>

namespace cg_lab2
{

    BeachLine::BeachLine() : mNil(new Arc), mRoot(mNil)
    {
        mNil->color = Arc::Color::Black; 
    }

    BeachLine::~BeachLine()
    {
        free(mRoot);
        delete mNil;
    }

    Arc* BeachLine::createArc(VoronoiDiagram::Site* site)
    {
        return new Arc
        {
            .parent = mNil, 
            .left = mNil,
            .right = mNil,
            .color = Arc::Color::Red,
            .prev = mNil,
            .next = mNil,
            .site = site, 
            .leftHalfEdge = nullptr,
            .rightHalfEdge = nullptr,
            .event = nullptr
        };
    }

    bool BeachLine::isEmpty() const
    {
        return isNil(mRoot);
    }

    bool BeachLine::isNil(const Arc* x) const
    {
        return x == mNil;
    }

    void BeachLine::setRoot(Arc* x)
    {
        mRoot = x; 
        mRoot->color = Arc::Color::Black;
    }

    Arc* BeachLine::getLeftmostArc() const
    {
        Arc* x = mRoot;

        while (!isNil(x->prev))
        {
            x = x->prev;
        }

        return x;
    }

    Arc* BeachLine::locateArcAbove(const Vector2& point, double l) const
    {
        Arc* node = mRoot;
        bool found = false;

        while (!found)
        {
            double breakpointLeft{ -std::numeric_limits<double>::infinity() };
            double breakpointRight{ std::numeric_limits<double>::infinity() };

            if (!isNil(node->prev))
            {
                breakpointLeft = computeBreakpoint(node->prev->site->point, node->site->point, l);
            }

            if (!isNil(node->next))
            {
                breakpointRight = computeBreakpoint(node->site->point, node->next->site->point, l);
            }

            if (point.x < breakpointLeft)
            {
                node = node->left;
            }
            else if (point.x > breakpointRight)
            {
                node = node->right;
            }
            else
            {
                found = true;
            }
        }

        return node;
    }

    void BeachLine::insertBefore(Arc* x, Arc* y)
    {
        if (isNil(x->left))
        {
            x->left = y;
            y->parent = x;
        }
        else
        {
            x->prev->right = y;
            y->parent = x->prev;
        }

        // Set the pointers
        y->prev = x->prev;

        if (!isNil(y->prev))
        {
            y->prev->next = y;
        }

        y->next = x;
        x->prev = y;

        insertFixup(y);
    }

    void BeachLine::insertAfter(Arc* x, Arc* y)
    {
        if (isNil(x->right))
        {
            x->right = y;
            y->parent = x;
        }
        else
        {
            x->next->left = y;
            y->parent = x->next;
        }

        y->next = x->next;
        
        if (!isNil(y->next))
        {
            y->next->prev = y;
        }

        y->prev = x;
        x->next = y;

        insertFixup(y);
    }

    void BeachLine::replace(Arc* x, Arc* y)
    {
        transplant(x, y);

        y->left = x->left;
        y->right = x->right;

        if (!isNil(y->left))
        {
            y->left->parent = y;
        }

        if (!isNil(y->right))
        {
            y->right->parent = y;
        }

        y->prev = x->prev;
        y->next = x->next;

        if (!isNil(y->prev))
        {
            y->prev->next = y;
        }

        if (!isNil(y->next))
        {
            y->next->prev = y;
        }

        y->color = x->color;
    }

    void BeachLine::remove(Arc* z)
    {
        Arc* y = z;
        Arc::Color yOriginalColor = y->color;
        Arc* x;

        if (isNil(z->left))
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (isNil(z->right))
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);

            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (yOriginalColor == Arc::Color::Black)
        {
            removeFixup(x);
        }

        if (!isNil(z->prev))
        {
            z->prev->next = z->next;
        }

        if (!isNil(z->next))
        {
            z->next->prev = z->prev;
        }
    }

    Arc* BeachLine::minimum(Arc* x) const
    {
        while (!isNil(x->left))
        {
            x = x->left;
        }

        return x;
    }

    void BeachLine::transplant(Arc* u, Arc* v)
    {
        if (isNil(u->parent))
        {
            mRoot = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }

        v->parent = u->parent;
    }

    void BeachLine::insertFixup(Arc* z)
    {
        while (z->parent->color == Arc::Color::Red)
        {
            if (z->parent == z->parent->parent->left)
            {
                Arc* y = z->parent->parent->right;

                // Case 1
                if (y->color == Arc::Color::Red)
                {
                    z->parent->color = Arc::Color::Black;
                    y->color = Arc::Color::Black;
                    z->parent->parent->color = Arc::Color::Red;
                    z = z->parent->parent;
                }
                else
                {
                    // Case 2
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        leftRotate(z);
                    }

                    // Case 3
                    z->parent->color = Arc::Color::Black;
                    z->parent->parent->color = Arc::Color::Red;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                Arc* y = z->parent->parent->left;

                // Case 1
                if (y->color == Arc::Color::Red)
                {
                    z->parent->color = Arc::Color::Black;
                    y->color = Arc::Color::Black;
                    z->parent->parent->color = Arc::Color::Red;
                    z = z->parent->parent;
                }
                else
                {
                    // Case 2
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(z);
                    }

                    // Case 3
                    z->parent->color = Arc::Color::Black;
                    z->parent->parent->color = Arc::Color::Red;
                    leftRotate(z->parent->parent);
                }
            }
        }

        mRoot->color = Arc::Color::Black;
    }

    void BeachLine::removeFixup(Arc* x)
    {

        while (x != mRoot && x->color == Arc::Color::Black)
        {
            Arc* w;
            if (x == x->parent->left)
            {
                w = x->parent->right;

                // Case 1
                if (w->color == Arc::Color::Red)
                {
                    w->color = Arc::Color::Black;
                    x->parent->color = Arc::Color::Red;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                // Case 2
                if (w->left->color == Arc::Color::Black && w->right->color == Arc::Color::Black)
                {
                    w->color = Arc::Color::Red;
                    x = x->parent;
                }
                else
                {
                    // Case 3
                    if (w->right->color == Arc::Color::Black)
                    {
                        w->left->color = Arc::Color::Black;
                        w->color = Arc::Color::Red;
                        rightRotate(w);
                        w = x->parent->right;
                    }

                    // Case 4
                    w->color = x->parent->color;
                    x->parent->color = Arc::Color::Black;
                    w->right->color = Arc::Color::Black;
                    leftRotate(x->parent);
                    x = mRoot;
                }
            }
            else
            {
                w = x->parent->left;

                // Case 1
                if (w->color == Arc::Color::Red)
                {
                    w->color = Arc::Color::Black;
                    x->parent->color = Arc::Color::Red;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                // Case 2
                if (w->left->color == Arc::Color::Black && w->right->color == Arc::Color::Black)
                {
                    w->color = Arc::Color::Red;
                    x = x->parent;
                }
                else
                {
                    // Case 3
                    if (w->left->color == Arc::Color::Black)
                    {
                        w->right->color = Arc::Color::Black;
                        w->color = Arc::Color::Red;
                        leftRotate(w);
                        w = x->parent->left;
                    }

                    // Case 4
                    w->color = x->parent->color;
                    x->parent->color = Arc::Color::Black;
                    w->left->color = Arc::Color::Black;
                    rightRotate(x->parent);
                    x = mRoot;
                } 
            }
        }

        x->color = Arc::Color::Black;
    }

    void BeachLine::leftRotate(Arc* x)
    {
        Arc* y = x->right;
        x->right = y->left;

        if (!isNil(y->left))
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (isNil(x->parent))
        {
            mRoot = y;
        }
        else if (x->parent->left == x)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void BeachLine::rightRotate(Arc* y)
    {
        Arc* x = y->left;
        y->left = x->right;

        if (!isNil(x->right))
        {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (isNil(y->parent))
        {
            mRoot = x;
        }
        else if (y->parent->left == y)
        {
            y->parent->left = x;
        }
        else
        {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;
    }

    double BeachLine::computeBreakpoint(const Vector2& point1, const Vector2& point2, double l) const
    {
        const double x1{ point1.x };
        const double y1{ point1.y };
        
        const double x2{ point2.x };
        const double y2{ point2.y };

        const double d1{ 1.0 / (2.0 * (y1 - l)) };
        const double d2{ 1.0 / (2.0 * (y2 - l)) };

        const double a{ d1 - d2 };
        const double b{ 2.0 * (x2 * d2 - x1 * d1) };
        const double c{ (y1 * y1 + x1 * x1 - l * l) * d1 - (y2 * y2 + x2 * x2 - l * l) * d2 };
        const double delta{ b * b - 4.0 * a * c };

        return (-b + std::sqrt(delta)) / (2.0 * a);
    }

    void BeachLine::free(Arc* x)
    {
        if (isNil(x))
        {
            return;
        }

        free(x->left);
        free(x->right);
        delete x;
    }

} // namespace cg_lab2