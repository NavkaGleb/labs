#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>

/* ---------------------------------------- class Circle ---------------------------------------- */
class Circle {
private:
	// variables
	double x;
	double y;
	double radius;

public:
	// constructor
	Circle() = default;
	Circle(double x, double y, double radius);

	// methods
	friend std::ostream& operator <<(std::ostream& out, Circle& circle);

};

/* ---------------------------------------- class Point3d ---------------------------------------- */
class Point3d {
private:
	// variables
	double x;
	double y;
	double z;

public:
	// constructor
	Point3d() = default;
	Point3d(double x, double y, double z);

	// methods
	friend std::ostream& operator <<(std::ostream& out, Point3d& point);

};

#endif // CIRCLE_H
