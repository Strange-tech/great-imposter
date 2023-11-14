// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include "camera.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <iostream>
#include <map>

/* 全局变量 */
const int mWidth = 1920;
const int mHeight = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = mWidth / 2, lastY = mHeight / 2;
bool firstMouse = true;
float aspect_ratio = (float)mWidth / (float)mHeight, near = 0.1f, far = 100.0f;

void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// int main(int argc, char * argv[]) {
//     /* Load GLFW and Create a Window */
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 向后兼容
//     // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁用窗口变化
//     auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

//     /* Check for Valid Context */
//     if (mWindow == nullptr) {
//         std::cout << "Failed to create GLFW window\n";
//         glfwTerminate();
//         return -1;
//     }

//     /* Create Context and Load OpenGL Functions */
//     glfwMakeContextCurrent(mWindow);
//     gladLoadGL();
//     fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

//     /* global settings */
//     glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    
//     glfwSetCursorPosCallback(mWindow, mouseCallback);
    
//     glfwSetScrollCallback(mWindow, scrollCallback);

//     glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // hide mouse

//     glEnable(GL_DEPTH_TEST); // enable depth test
//     glDepthFunc(GL_LESS);

//     glEnable(GL_BLEND); // color blending
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     // vertex
//     float cubeVertices[] = {
//         // positions          // texture Coords
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//          0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//          0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//     };
//     float transparentVertices[] = {
//         // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
//         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//         0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
//         1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

//         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//         1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//         1.0f,  0.5f,  0.0f,  1.0f,  0.0f
//     };
//     float planeVertices[] = {
//         // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//          5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//         -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//         -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

//          5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//         -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//          5.0f, -0.5f, -5.0f,  2.0f, 2.0f                                
//     };
//     // transparent window locations
//     std::vector<glm::vec3> windows {
//         glm::vec3(-1.5f, 0.0f, -0.48f),
//         glm::vec3( 1.5f, 0.0f, 0.51f),
//         glm::vec3( 0.0f, 0.0f, 0.7f),
//         glm::vec3(-0.3f, 0.0f, -2.3f),
//         glm::vec3( 0.5f, 0.0f, -0.6f)
//     };

//     // ----------------------------- 正方体 ----------------------------- //
//     unsigned int cubeVAO, cubeVBO;
//     glGenVertexArrays(1, &cubeVAO);
//     glGenBuffers(1, &cubeVBO);    
//     glBindVertexArray(cubeVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glBindVertexArray(0);

//     // ----------------------------- 平面 ----------------------------- //
//     unsigned int planeVAO, planeVBO;
//     glGenVertexArrays(1, &planeVAO);
//     glGenBuffers(1, &planeVBO);
//     glBindVertexArray(planeVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glBindVertexArray(0);

//     // ----------------------------- 透明平面 ----------------------------- //
//     unsigned int transparentVAO, transparentVBO;
//     glGenVertexArrays(1, &transparentVAO);
//     glGenBuffers(1, &transparentVBO);
//     glBindVertexArray(transparentVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glBindVertexArray(0);

//     // load textures
//     unsigned int cubeTexture  = loadTexture("/Users/luziang/Desktop/Glitter/Glitter/Sources/marble.jpg");
//     unsigned int floorTexture = loadTexture("/Users/luziang/Desktop/Glitter/Glitter/Sources/metal.png");
//     unsigned int transparentTexture = loadTexture("/Users/luziang/Desktop/Glitter/Glitter/Sources/blending_transparent_window.png");

//     Shader shader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/depth_testing.vert", 
//         "/Users/luziang/Desktop/Glitter/Glitter/Shaders/depth_testing.frag");
//     // Shader singleColorShader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/depth_testing.vert", 
//         // "/Users/luziang/Desktop/Glitter/Glitter/Shaders/single_color.frag");

//     // activate shader
//     shader.use();
//     shader.setInt("texture1", 0);
//     // singleColorShader.use();

//     // Rendering Loop
//     while (glfwWindowShouldClose(mWindow) == false) {
//         float currentFrame = static_cast<float>(glfwGetTime());
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;
        
//         processInput(mWindow);

//         std::map<float, glm::vec3> sorted;
//         for (unsigned int i = 0; i < windows.size(); i++) {
//             float distance = glm::length(camera.Position - windows[i]);
//             sorted[distance] = windows[i];
//         }

//         // Background Fill Color
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // set uniforms
//         // singleColorShader.use();
//         glm::mat4 model = glm::mat4(1.0f);
//         glm::mat4 view = camera.GetViewMatrix();
//         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, 0.1f, 100.0f);
//         // singleColorShader.setMat4("view", view);
//         // singleColorShader.setMat4("projection", projection);

//         shader.use();
//         shader.setMat4("view", view);
//         shader.setMat4("projection", projection);

//         // floor
//         glBindVertexArray(planeVAO);
//         glBindTexture(GL_TEXTURE_2D, floorTexture);
//         shader.setMat4("model", glm::mat4(1.0f));
//         glDrawArrays(GL_TRIANGLES, 0, 6);
//         glBindVertexArray(0);

//         // cubes
//         glBindVertexArray(cubeVAO);
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, cubeTexture);
//         model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//         shader.setMat4("model", model);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//         model = glm::mat4(1.0f);
//         model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//         shader.setMat4("model", model);
//         glDrawArrays(GL_TRIANGLES, 0, 36);

//         // glass
//         glBindVertexArray(transparentVAO);
//         glBindTexture(GL_TEXTURE_2D, transparentTexture);  
//         for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
//             model = glm::mat4(1.0f);
//             model = glm::translate(model, it->second);              
//             shader.setMat4("model", model);
//             glDrawArrays(GL_TRIANGLES, 0, 6);
//         }
        
//         // Flip Buffers and Draw
//         glfwSwapBuffers(mWindow);
//         glfwPollEvents();
//     }   
    
//     glDeleteVertexArrays(1, &cubeVAO);
//     glDeleteVertexArrays(1, &planeVAO);
//     glDeleteBuffers(1, &cubeVBO);
//     glDeleteBuffers(1, &planeVBO);

//     glfwTerminate();
//     return 0;
// }


int main(int argc, char * argv[]) {
    /* Load GLFW and Create a Window */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 向后兼容
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁用窗口变化
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    /* Check for Valid Context */
    if (mWindow == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    /* Create Context and Load OpenGL Functions */
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    /* global settings */
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    
    glfwSetCursorPosCallback(mWindow, mouseCallback);
    
    glfwSetScrollCallback(mWindow, scrollCallback);

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // hide mouse

    glEnable(GL_DEPTH_TEST); // enable depth test

    // vertex
    float cubeVertices[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // float planeVertices[] = {
    //     // positions          // texture Coords 
    //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    //     -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    //      5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    // };
    // float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //     // positions   // texCoords
    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //     -1.0f, -1.0f,  0.0f, 0.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,

    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,
    //      1.0f,  1.0f,  1.0f, 1.0f
    // };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // ----------------------------- 正方体 ----------------------------- //
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);    
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // ----------------------------- 天空盒 ----------------------------- //
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // ----------------------------- 平面 ----------------------------- //
    // unsigned int planeVAO, planeVBO;
    // glGenVertexArrays(1, &planeVAO);
    // glGenBuffers(1, &planeVBO);
    // glBindVertexArray(planeVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glBindVertexArray(0);

    // ----------------------------- quad VAO ----------------------------- //
    // unsigned int quadVAO, quadVBO;
    // glGenVertexArrays(1, &quadVAO);
    // glGenBuffers(1, &quadVBO);
    // glBindVertexArray(quadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // glBindVertexArray(0);


    // load textures
    std::vector<std::string> faces {
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/right.jpg",
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/left.jpg",
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/top.jpg",
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/bottom.jpg",
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/front.jpg",
        "/Users/luziang/Desktop/Glitter/Glitter/Sources/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces); 
    unsigned int cubeTexture  = loadTexture("/Users/luziang/Desktop/Glitter/Glitter/Sources/container.png");
    // unsigned int floorTexture = loadTexture("/Users/luziang/Desktop/Glitter/Glitter/Sources/metal.png");

    Shader shader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/cube.vert", 
        "/Users/luziang/Desktop/Glitter/Glitter/Shaders/cube.frag");
    Shader skyboxShader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/skybox.vert", 
        "/Users/luziang/Desktop/Glitter/Glitter/Shaders/skybox.frag");
    // Shader screenShader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/framebuffers_screen.vert", 
    //     "/Users/luziang/Desktop/Glitter/Glitter/Shaders/framebuffers_screen.frag");

    // activate shader
    shader.use();
    shader.setInt("skybox", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    // screenShader.use();
    // screenShader.setInt("screenTexture", 0);

    // framebuffer configuration
    // unsigned int framebuffer;
    // glGenFramebuffers(1, &framebuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // // create a color attachment texture
    // unsigned int textureColorbuffer;
    // glGenTextures(1, &textureColorbuffer);
    // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // // create another renderbuffer object for depth and stencil attachment
    // unsigned int rbo;
    // glGenRenderbuffers(1, &rbo);
    // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    // // check if it's correct
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //     std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << '\n';
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // draw as wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(mWindow);

        // bind to framebuffer and draw scene as we normally would to color texture 
        // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        // Background Fill Color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set uniforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, 0.1f, 100.0f);

        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("cameraPos", camera.Position);

        // floor
        // glBindVertexArray(planeVAO);
        // glBindTexture(GL_TEXTURE_2D, floorTexture);
        // shader.setMat4("model", glm::mat4(1.0f));
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0);

        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // bind back to default framebuffer
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glDisable(GL_DEPTH_TEST);
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        // glClear(GL_COLOR_BUFFER_BIT);
        // screenShader.use();
        // glBindVertexArray(quadVAO);
        // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   
    
    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}


unsigned int loadTexture(const char *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << '\n';
        stbi_image_free(data);
    }
    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << '\n';
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}  

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

}

void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
