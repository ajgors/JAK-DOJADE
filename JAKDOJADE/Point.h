#pragma once

struct Point {
    int y;
    int x;

    Point()
        : y(0), x(0)
    {
    }

    Point(int y, int x)
        : y(y), x(x)
    {
    }

    Point(const Point& other) {
        y = other.y;
        x = other.x;
    }

    Point& operator=(const Point& other) {
        y = other.y;
        x = other.x;
        return *this;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};


struct hashPoint {
    size_t operator()(const Point& p) const {
        return  ((p.x + p.y) * (p.x + p.y + 1) / 2) + p.y;
    }
};
