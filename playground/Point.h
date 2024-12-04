#ifndef POINT_H
#define POINT_H
#include <GL/glew.h>
#include <glfw3.h>

//for saving points in a 3D room
struct Point {
    float x;
    float y;
    float z;

    Point() : x(0), y(0), z(0) {}

    Point(float xp, float yp, float zp) : x(xp), y(yp), z(zp) {}

    [[nodiscard]] bool equals(const Point &point) const {       //return should not be ignored 
        return ((x == point.x) && (y == point.y) && (z == point.z));
    }
};

#endif
