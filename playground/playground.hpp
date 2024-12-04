#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <common/texture.hpp>
#include "RenderObject.h"
#include "glfw3.h"
#include <common/objloader.hpp>

using namespace glm;
using namespace std;

//shader program ID
GLuint programID;

//vertex buffer variables
GLuint vertexBuffer[2];
GLuint VertexArrayID;

//Model-View-Projection matrices and IDs
GLuint MatrixID;
mat4 MVP;
GLuint MatrixIDMV;
mat4 MV;

//texture variables
GLuint texID;
GLuint uvBuffer;
GLuint Texture;

//rendering object and associated data
RenderObject renderingObject;
vector<vec3> vertices;
vector<vec2> uvs;
vector<vec3> normals;

//global variables to handle the object pose
float cameraZoom{ 35 };
float curr_x;
float curr_y;
float curr_angle;
float tilt;
bool autoRotate;
bool isPressed;

int main();

//updates animation loop
void updateAnimationLoop();

//handles user input
void handleInput();

//handles rotation toggle
void handleRotationToggle();

//initializes the window using GLFW and GLEW
bool initializeWindow();

//callback for resizing window
void framebuffer_size_callback(GLFWwindow*, int width, int height);

//updates the MVP, transform with the current pose
bool updateMVPTransformation();

//initializes the vertex buffer array and binds it to OpenGL
bool initializeVertexBuffer();

//frees all resources from the vertex buffer
bool cleanupVertexBuffer();

//closes the OpenGL window and terminates GLFW
bool closeWindow();

#endif
