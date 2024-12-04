#include "RenderObject.h"
#include <common/texture.hpp>
#include <GL/glew.h>
#include <glfw3.h>

using namespace glm;
using namespace std;

//constructor initializes the model matrix
RenderObject::RenderObject() : model(mat4(1.0f)) {}

//destructor
RenderObject::~RenderObject() = default;

//initialize Vertex Array Object (VAO)
void RenderObject::InitializeVAO() {
    glGenVertexArrays(1, &VertexArrayID);
}

//set the vertices for the object
//binds the VAO and generates a Vertex Buffer Object (VBO)
void RenderObject::SetVertices(vector<vec3> vertices) {
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexBuffer);
    VertexBufferSize = vertices.size() * sizeof(vec3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

//set the normals for the object
void RenderObject::SetNormals(vector<vec3> normals) {
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

//set texture with RAW data
//set the texture for the object using raw texture data with GLubyte
//generates a texture and binds it
//saves the texture coordinates (UV) into the VBO 
void RenderObject::SetTexture(vector<vec2> uvBufferData, GLubyte textureData[]) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTextureStorage2D(texID, 4, GL_R8, 8, 8);
    glTextureSubImage2D(texID, 0, 0, 0, 8, 8, GL_RED, GL_UNSIGNED_BYTE, textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvBufferData.size() * sizeof(vec2), &uvBufferData[0], GL_STATIC_DRAW);
}


//set the texture for the object using a BMP image file
void RenderObject::SetTexture(vector<vec2> uvBufferData, const string& bmpPath) {
    glGenTextures(1, &texID);
    texID = loadBMP_custom(bmpPath.c_str());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvBufferData.size() * sizeof(vec2), &uvBufferData[0], GL_STATIC_DRAW);
}

//draw the object on the screen
void RenderObject::DrawObject() const {
    //enable vertex attributes for vertices, normals, and texture coordinates
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //bind vertex buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //bind vertex buffer for normals
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //bind texture and set texture sampler
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(textureSamplerID, 0);

    //bind vertex buffer for texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //draw the object using triangles
    glDrawArrays(GL_TRIANGLES, 0, VertexBufferSize / sizeof(vec3));

    //disable vertex attributes to ensure that they are not being reused
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

//get triangle normals for a specific vertex
vector<vec3> RenderObject::getTriangleNormals(Point vertex, const vector<Triangle>& triangles) {
    vector<vec3> returnValue;
    for (auto t : triangles) {
        if (t.v1.equals(vertex) || t.v2.equals(vertex) || t.v3.equals(vertex)) {
            vec3 normal = { t.normal.x, t.normal.y, t.normal.z };
            returnValue.push_back(normal);
        }
    }
    return returnValue;
}

//calculate the mean vector for a set of vectors
    vec3 RenderObject::calculateMeanVector(const vector<vec3>& vector) {
    vec3 mean = { 0, 0, 0 };
    for (auto element : vector) {
        mean += element;
    }
    normalize(mean);
    return mean;
}

//compute normals for each vertex using triangle data
void RenderObject::computeTrianglesVertices(vector<vec3>& vertices, vector<vec3>& normals) {
    vector<Triangle> triangles;

    // Create triangles with normals from vertices
    for (int i = 0; i < vertices.size(); i += 3) {
        vec3 edge1 = vertices[i + 1] - vertices[i];
        vec3 edge2 = vertices[i + 2] - vertices[i];
        vec3 triangleNormal = normalize(cross(edge1, edge2));

        Point v1 = { vertices[i][0], vertices[i][1], vertices[i][2] };
        Point v2 = { vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2] };
        Point v3 = { vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2] };
        Point normal = { triangleNormal[0], triangleNormal[1], triangleNormal[2] };

        Triangle currentTriangle = { normal, v1, v2, v3 };
        triangles.push_back(currentTriangle);
    }

    // Calculate normals for each vertex
    for (vec3 vertex : vertices) {
        normals.push_back(calculateMeanVector(getTriangleNormals({ vertex[0], vertex[1], vertex[2] }, triangles)));
    }
}