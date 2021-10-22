#include <GL/gl.h>
#include <cmath>
#include "functions.h"

constexpr double PI = 3.14;

// -------------------------------------------------- CLASS CIRCLE --------------------------------------------------
Circle::Circle(double _x, double _y, double _radius, float _speed) {
    x = _x;
    y = _y;
    radius = _radius;
    speed = _speed;
}

double Circle::getX() {
    return this->x;
}

double Circle::getY() {
    return y;
}

double Circle::getRadius() {
    return radius;
}

float Circle::getAngle() {
    return angle;
}

void Circle::updateAngle() {
    if (angle > 360)
        angle = 0;
    angle += speed;
}

void Circle::draw(unsigned int type) {

    glBegin(type);

        double ang;
        for (int i = 0; i < 360; i++) {
            ang = double(i * PI / 180);
            glVertex2d(sin(ang) * radius + x, cos(ang) * radius + y);
        }

    glEnd();

}

// -------------------------------------------------- CLASS POINT --------------------------------------------------
Point::Point(double _x, double _y) {
    x = _x;
    y = _y;
}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}

void Point::updateLocation(Circle circle) {
    x = sin(double(circle.getAngle() * PI / 180)) * circle.getRadius() + circle.getX();
    y = cos(double(circle.getAngle() * PI / 180)) * circle.getRadius() + circle.getY();
}

void Point::draw() {

    glBegin(GL_POLYGON);

    double angle;
    for (int i = 0; i < 360; i++) {
        angle = double(i * PI / 180);
        glVertex2d(sin(angle) * radius + x, cos(angle) * radius + y);
    }

    glEnd();

}

// -------------------------------------------------- CLASS CURVE --------------------------------------------------
void Curve::pushPoint(Point point) {
    points.push_back(point);
}

void Curve::clear() {
    points.clear();
}

void Curve::draw() {

    glBegin(GL_LINE_STRIP);

        for (auto point : points)
            glVertex2d(point.getX(), point.getY());

    glEnd();

}

// other functions
void drawLine(double x1, double y1, double x2, double y2) {

    glBegin(GL_LINES);

        glVertex2d(x1, y1);
        glVertex2d(x2, y2);

    glEnd();

}
