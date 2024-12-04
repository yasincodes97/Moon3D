#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Point.h"
#include "Triangle.h"

using namespace std;
using namespace glm;

class RenderObject {

public:
    RenderObject();

    virtual ~RenderObject();

    void InitializeVAO();

    void SetVertices(vector<vec3>);                         //vector are dynamical arrays saving vecs from GLM with templates <>

    void SetNormals(vector<vec3>);

    void SetTexture(vector<vec2>, GLubyte textureData[]);

    void SetTexture(vector<vec2>, const string &bmpPath);

    void DrawObject() const;


    int VertexBufferSize{};

    mat4 model;

    GLuint VertexArrayID{};
    GLuint vertexBuffer{};
    GLuint normalBuffer{};
    GLuint uvBuffer{};
    GLuint texID{};
    GLuint textureSamplerID{};

    static void computeTrianglesVertices(vector<vec3> &vertices, vector<vec3> &normals);

    static vector<vec3> getTriangleNormals(Point vertex, const vector<Triangle> &triangles);

    static vec3 calculateMeanVector(const vector<vec3> &);

    //void SetupBuffer(GLuint& buffer, GLuint index, const vector<vec3>& data);
};

#endif