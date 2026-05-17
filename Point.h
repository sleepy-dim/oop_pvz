#ifndef POINT_H
#define POINT_H
#include <cmath>

class Point
{
public:
    float x;
    float y;

    Point(float x, float y) : x(x), y(y) {}
    Point() : x(0), y(0) {}

    // Operator Overloading for Rubric Requirement
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Friend Function that uses the - operator (Useful for collision/distance checking)
    friend float distance(const Point& p1, const Point& p2) {
        Point diff = p1 - p2; // Utilizes the overloaded - operator
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    // Friend Function + Operator Overloading
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

#endif
