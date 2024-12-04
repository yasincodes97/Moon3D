#include <iostream>
#include "playground.hpp"
#include <glfw3.h>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <thread>
#include <common/shader.hpp>

//used openGL libraries: GLew, GLfw, GLm, 

using namespace glm;
using namespace std;


//window in which the scene is rendered
GLFWwindow* window;

int main() {
    //initialize window and vertex buffer
    if (!initializeWindow()) return -1;
    if (!initializeVertexBuffer()) return -1;
    

    //load shaders
    programID = LoadShaders("../resources/SimpleVertexShader.vertexshader",
                            "../resources/SimpleFragmentShader.fragmentshader");

    //set initial parameters
    curr_x = 0;
    curr_y = 0;
    curr_angle = 0;
    tilt = -0.2;
    isPressed = false;

    //get uniform locations
    MatrixIDMV = glGetUniformLocation(programID, "MV");
    MatrixID = glGetUniformLocation(programID, "MVP");
    updateMVPTransformation();

    //depth test, ensures that objects which are in the background are not rendered at first
    glEnable(GL_DEPTH_TEST);

    //main loop invokes updateAnimationloop()
    do {
        updateAnimationLoop();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    //cleanup buffers, shaders, window
    cleanupVertexBuffer();
    glDeleteProgram(programID);
    closeWindow();

    return 0;
}

/*void initializeBackground() {
    float vertices[] = {
        // Positions         // Texture Coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    };
    
    glGenVertexArrays(1, &backgroundVAO);
    glGenBuffers(1, &backgroundVBO);

    glBindVertexArray(backgroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
*/

//update state and render frame
void updateAnimationLoop() {
    glUseProgram(programID);                                //activates shader
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    //deletes color buffers, clears screen

    handleInput();

    if (autoRotate) curr_angle += 0.015;
    
    //update the MVP matrices
    updateMVPTransformation();   

    //passing the matrices to the shader
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);      
    glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

    //activate and bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glUniform1i(texID, 0);

    //drawing the object
    renderingObject.DrawObject();

    //handles the user inputs and updates the window
    glfwSwapBuffers(window);
    glfwPollEvents();
}

//handle user input with GLFWGetKey
void handleInput() {
    //camera zoom with W and S
    if (glfwGetKey(window, GLFW_KEY_UP)) cameraZoom -= 0.4;
    else if (glfwGetKey(window, GLFW_KEY_DOWN)) cameraZoom += 0.4;
    
    //object rotation with A and D
    if (glfwGetKey(window, GLFW_KEY_LEFT)) curr_angle -= 0.01;
    else if (glfwGetKey(window, GLFW_KEY_RIGHT)) curr_angle += 0.01;

    //object tilt with Q and E
    if (glfwGetKey(window, GLFW_KEY_Z)) tilt += 0.01;
    else if (glfwGetKey(window, GLFW_KEY_X)) tilt -= 0.01;

    handleRotationToggle();
}

//toggle rotation
void handleRotationToggle() {
    if (!isPressed && glfwGetKey(window, GLFW_KEY_R)) {
        autoRotate = !autoRotate;
        isPressed = true;
    }
    else if (isPressed && glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        isPressed = false;
    }
}

//initialize GLFW window
bool initializeWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    //set GLFW window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create GLFW window
    window = glfwCreateWindow(1920, 1080, "3D Moon", nullptr, nullptr);  //full HD
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return false;
    }

    //make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //initialize GLEW
    glewExperimental = true;             //needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }

    //ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //set background color
    glClearColor(0.0, 0.0, 0.0, 1.0);
    return true;
}

// GLFW framebuffer is called when size of window is changed
void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    //keep display dimension 16:9
    int game_width;
    int game_height;
    int half_gap_x{};       //initialize with zero
    int half_gap_y{};      //same here

    if (width > height) {
        //width is larger
        game_width = { static_cast<int>(static_cast<float>(height) * (16.0f / 9.0f)) };
        game_height = { height };
        half_gap_x = { (width - game_width) / 2 };
    }
    else {
        //height is larger
        game_width = { width };
        game_height = { static_cast<int>(static_cast<float>(width) * (9.0f / 16.0f)) };
        half_gap_y = { (height - game_height) / 2 };
    }

    glViewport(half_gap_x, half_gap_y, game_width, game_height);
}

//update Model-View-Projection transformation
//MVP-matrix is product of model-, projection-, view-matrix and is passed to the shaders
bool updateMVPTransformation() {
    //projection matrix
    mat4 projection = perspective(radians(30.0f), 4.0f / 3.0f, 0.1f, 10000.0f);

    //view matrix
    mat4 view = lookAt(
    vec3(0, 0, cameraZoom),       //camera position
    vec3(0, 0, 0),               //point of view, the origin
    vec3(0, 1, 0)               //upside-down
    );

    //model matrix
    mat4 model = mat4(1.0f);  //start with the identity matrix

    //moon tilt and rotation
    model = rotate(model, tilt, vec3(0.0f, 0.0f, 1.0f));

    //independent moon rotation around Y-axis (custom speed)
    //static float moon_rotation = 0.0f;
    //moon_rotation += 0.02f;                                       //control the rotation speed
    //model = rotate(model, moon_rotation, vec3(0.0f, 1.0f, 0.0f));

    //apply rotation around the Y-axis for a 360-degree view
    model = rotate(model, curr_angle, vec3(0.0f, 1.0f, 0.0f));

    //translation
    model = translate(model, vec3(curr_x, curr_y, 0.0f));

    //MVP multiplication of the three matrices
    MVP = projection * view * model;  //matrix multiplication is inversed, transformation goes from bottom to top
    MV = view * model;               //MV matrix for the light

    return true;
}

//initialize vertex buffer and loads .obj file
//load and bind the texture 
bool initializeVertexBuffer() {
    //loads the uniform-texture
    glGetUniformLocation(programID, "../resources/MyTextureSampler");

    //render object inizialize
    renderingObject = RenderObject();
    renderingObject.InitializeVAO();

    //generates vertex array
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //load object data
    loadOBJ("../resources/moon.obj", vertices, uvs, normals);

    //set rendering object data
    renderingObject.SetVertices(vertices);
    renderingObject.SetNormals(normals);
    RenderObject::computeTrianglesVertices(vertices, normals);

    //set texture data
    renderingObject.textureSamplerID = glGetUniformLocation(programID, "MyTextureSampler");
    vector<vec2> uvBufferData = uvs;
    renderingObject.SetTexture(uvBufferData,"../resources/moon.bmp");


    //generate buffers for vertices and normals
    glGenBuffers(2, vertexBuffer);

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    return true;
}

//cleanup vertex-buffer, texture, UV-buffer, vertex-array
//vertex-buffer: data of the vertices
//veretx-array: describes this data
//shaders take these buffers
bool cleanupVertexBuffer() {
    glDeleteBuffers(2, vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    return true;
}

//close GLFW window
bool closeWindow() {
    glfwTerminate();
    return true;
}