#ifndef COORDINATE_H
#define COORDINATE_H
#include <cmath>
#include <iostream>

class coordinate
{
public:
    float x;
    float y;

    coordinate(float x, float y) : x(x), y(y) {}
    coordinate() : x(0), y(0) {}

    // Operator Overloading for Rubric Requirement
    coordinate operator+(const coordinate& other) const {
        return coordinate(x + other.x, y + other.y);
    }

    coordinate operator-(const coordinate& other) const {
        return coordinate(x - other.x, y - other.y);
    }

    bool operator==(const coordinate& other) const {
        return (x == other.x && y == other.y);
    }

    coordinate& operator+=(const coordinate& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Friend Function that uses the - operator (Useful for collision/distance checking)
    friend float distance(const coordinate& p1, const coordinate& p2) {
        coordinate diff = p1 - p2; // Utilizes the overloaded - operator
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    // Friend Function + Operator Overloading
    friend std::ostream& operator<<(std::ostream& os, const coordinate& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

#endif
