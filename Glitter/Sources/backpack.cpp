// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>

// Standard Headers
#include <iostream>
#include <cmath>

// Local Headers
#include "shader.h"
#include "camera.h"
#include "model.h"

#define PI 3.1415926

/* 全局变量 */
const int mWidth = 800;
const int mHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = mWidth / 2, lastY = mHeight / 2;
bool firstMouse = true;
float aspect_ratio = (float)mWidth / (float)mHeight, near = 0.1f, far = 100.0f;

void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);

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

    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis
    
    Shader ourShader("/Users/luziang/Desktop/Glitter/Glitter/Shaders/model_loading.vert", 
        "/Users/luziang/Desktop/Glitter/Glitter/Shaders/model_loading.frag");
    ourShader.use();

    Model ourModel("/Users/luziang/Desktop/Glitter/Glitter/Sources/backpack/backpack.obj");

    Box boundingbox = ourModel.getBoundingBox();
    // std::cout << boundingbox.min.x <<' '<< boundingbox.min.y <<' '<< boundingbox.min.z << '\n';
    // std::cout << boundingbox.max.x <<' '<< boundingbox.max.y <<' '<< boundingbox.max.z << '\n';
    // std::cout << boundingbox.center.x <<' '<< boundingbox.center.y <<' '<< boundingbox.center.z << '\n';

    std::vector<glm::vec3> cam_position_array;
    float radius = std::sqrt(std::pow(boundingbox.min.x - boundingbox.max.x, 2) + std::pow(boundingbox.min.y - boundingbox.max.y, 2) + std::pow(boundingbox.min.z - boundingbox.max.z, 2));
    radius *= 2;
    int vertical_segments = 5, horizon_segments = 10;
    for(float i = 0; i <= PI; i += PI / vertical_segments) {
        for(float j = 0; j <= 2*PI; j += 2*PI / horizon_segments) {
            glm::vec3 cam_direction = glm::vec3(radius*std::sin(j)*std::cos(i), radius*std::sin(j)*std::sin(i), radius*std::cos(j));
            glm::vec3 cam_position = boundingbox.center + cam_direction;
            // std::cout << cam_position.x <<' '<< cam_position.y <<' '<< cam_position.z << '\n';
            cam_position_array.push_back(cam_position);
        }
    }
    std::cout << cam_position_array.size() << '\n';
     

    // directional light
    ourShader.setVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);
    ourShader.setVec3("dirlight.ambient", 0.2f, 0.2f, 0.2f);
    ourShader.setVec3("dirlight.diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);

    // point lights
    glm::vec3 lightPositions[] = {
        glm::vec3( 3.0f ),
        glm::vec3( -3.0f )
    };

    ourShader.setVec3("pointlights[0].position", lightPositions[0]);
    ourShader.setVec3("pointlights[0].ambient", 0.2f, 0.2f, 0.2f);
    ourShader.setVec3("pointlights[0].diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("pointlights[0].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointlights[0].constant", 1.0f);
    ourShader.setFloat("pointlights[0].linear", 0.09f);
    ourShader.setFloat("pointlights[0].quadratic", 0.032f);

    ourShader.setVec3("pointlights[1].position", lightPositions[1]);
    ourShader.setVec3("pointlights[1].ambient", 0.2f, 0.2f, 0.2f);
    ourShader.setVec3("pointlights[1].diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("pointlights[1].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointlights[1].constant", 1.0f);
    ourShader.setFloat("pointlights[1].linear", 0.09f);
    ourShader.setFloat("pointlights[1].quadratic", 0.032f);

    // Rendering Loop
    // int cnt = 0, total = cam_position_array.size() * 100;
    for(int i = 0; i < cam_position_array.size(); i++) {
        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.use();

        camera.setPosition(cam_position_array[i]);
        camera.lookAtTarget(boundingbox.center);
        
        glm::mat4 view = camera.GetViewMatrix(); // update view matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, near, far);

        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("viewPos", camera.Position);
        
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        // cv::Mat img(mWidth, mHeight, CV_8UC3);
        // glReadPixels(0, 0, img.cols, img.rows, GL_RGB, GL_UNSIGNED_BYTE, img.data);
        // std::string path = "output" + std::to_string(i) + ".png";
        // cv::imwrite(path, img);

        // save image
        int mac_width = mWidth*2, mac_height = mHeight*2;
        GLubyte* pPixelData;
        pPixelData = (GLubyte*)malloc(mac_width * mac_height * 4); // 分配内存
        if (pPixelData == 0)
            return 0;
        glReadBuffer(GL_FRONT); // 保存窗口渲染的结果
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 解压窗口数据结构
        glReadPixels(0, 0, mac_width, mac_height, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData); // 存储像素数据
 
        cv::Mat img;
        std::vector<cv::Mat> imgPlanes;
        img.create(mac_height, mac_width, CV_8UC3); // 确定图片通道和尺寸
        cv::split(img, imgPlanes); // 将图像按照通道数拆分，三个单通道序列
 
        for(int i = 0; i < mac_height; i ++) {
            unsigned char* plane0Ptr = imgPlanes[0].ptr<unsigned char>(i); //B
            unsigned char* plane1Ptr = imgPlanes[1].ptr<unsigned char>(i); //G
            unsigned char* plane2Ptr = imgPlanes[2].ptr<unsigned char>(i); //R
            // opencv里面以BGR存储的，所以需要改变顺序保存
            for(int j = 0; j < mac_width; j ++) {
                int k = 4 * (i * mac_width + j); // RGBA数据结构，不需要A，跳过，所以步长乘以4
                plane2Ptr[j] = pPixelData[k];
                plane1Ptr[j] = pPixelData[k+1];
                plane0Ptr[j] = pPixelData[k+2];
            }
        }
        cv::merge(imgPlanes, img); // 合并多通道图像
        cv::flip(img, img ,0); // 反转图像，因为opengl和opencv的坐标系y轴是相反的
        // cv::cvtColor(img, img, cv::COLOR_RGB2GRAY); // 转换为灰度图
        // cv::namedWindow("openglGrab");
        // cv::imshow("openglGrab", img);
        // cv::waitKey();
        std::string path = "output" + std::to_string(i) + ".png";
        cv::imwrite(path, img);

    }
    // while (glfwWindowShouldClose(mWindow) == false) {
    //     float currentFrame = static_cast<float>(glfwGetTime());
    //     deltaTime = currentFrame - lastFrame;
    //     lastFrame = currentFrame;
        
    //     processInput(mWindow);

    //     // Background Fill Color
    //     glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    //     ourShader.use();

    //     if(cnt < total) {
    //         camera.setPosition(cam_position_array[cnt/100]);
    //         camera.lookAtTarget(boundingbox.center);
    //     }
        
    //     glm::mat4 view = camera.GetViewMatrix(); // update view matrix
    //     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, near, far);

    //     ourShader.setMat4("view", view);
    //     ourShader.setMat4("projection", projection);
    //     ourShader.setVec3("viewPos", camera.Position);
        
    //     // render the loaded model
    //     glm::mat4 model = glm::mat4(1.0f);
    //     model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    //     model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
    //     ourShader.setMat4("model", model);
    //     ourModel.Draw(ourShader);

    //     // Flip Buffers and Draw
    //     glfwSwapBuffers(mWindow);
    //     glfwPollEvents();

    //     if(cnt < total) cnt++;
    // }

    glfwTerminate();
    return 0;
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

