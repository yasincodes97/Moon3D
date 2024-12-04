#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <GL/glew.h>
#include <glfw3.h>
#include "Point.h"

//for calculating the normal vertices in computeTriangleVertices function
//needed for calculating the light
struct Triangle {
    Point normal;
    Point v1;
    Point v2;
    Point v3;

    Triangle(Point _normal, Point _v1, Point _v2, Point _v3) :
            normal(_normal), v1(_v1), v2(_v2), v3(_v3) {}
};

#endif
