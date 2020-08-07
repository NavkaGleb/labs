#ifndef OPEN_GL_FUNCTIONS_H
#define OPEN_GL_FUNCTIONS_H

#include <vector>

class Circle {
private:

    double x = 0;
    double y = 0;
    double radius = 1;
    float speed = 1;
    float angle = 0;

public:

    Circle() = default;
    Circle(double _x, double _y, double _radius, float _speed);

    double getX();
    double getY();
    double getRadius();
    float getAngle();

    void updateAngle();
    void draw(unsigned int type);

};

class Point {
private:

    double x = 0;
    double y = 0;
    double radius = 5;

public:

    Point() = default;
    Point(double _x, double _y);

    double getX();
    double getY();

    void updateLocation(Circle circle);
    void draw();
};

class Curve {
private:

    std::vector<Point> points;

public:

    Curve() = default;

    void pushPoint(Point point);
    void clear();

    void draw();

};

// other functions
void drawLine(double x1, double y1, double x2, double y2);

#endif //OPEN_GL_FUNCTIONS_H