#include "Geometry.h"

/* ---------------------------------------- class Circle ---------------------------------------- */

// constructor

Circle::Circle(double x, double y, double radius)
	: x(x), y(y), radius(radius) {}

// methods

std::ostream& operator <<(std::ostream& out, Circle& circle) {

	out << "center(" << circle.x << ", " << circle.y << "), radius = " << circle.radius;
	return out;

}

/* ---------------------------------------- class Point3d ---------------------------------------- */

Point3d::Point3d(double x, double y, double z)
	: x(x), y(y), z(z) {}

// methods

std::ostream& operator <<(std::ostream& out, Point3d& point) {

	out << "(" << point.x << ", " << point.y << ", " << point.z << ")";
	return out;

}