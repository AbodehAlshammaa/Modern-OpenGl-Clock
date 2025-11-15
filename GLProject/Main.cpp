// GLProject.cpp : 3D Wooden Clock with Glass Cover - REAL TIME
// -------------------------------------------------

#include <iostream>
#include <chrono>
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon.cpp"
#include "PolygonLine.cpp"
using namespace glm;

int width = 1920;
int height = 1080;

vec3 cameraPos = vec3(0.0f, 0.0f, 4.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

// ------------------------------------------------------------
// Get real system time components
// ------------------------------------------------------------
void getCurrentTime(int& hours, int& minutes, int& seconds, float& milliseconds)
{
    // Using C++ chrono for precise time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;

#ifdef _WIN32
    localtime_s(&local_tm, &now_time_t);
#else
    localtime_r(&now_time_t, &local_tm);
#endif

    hours = local_tm.tm_hour;
    minutes = local_tm.tm_min;
    seconds = local_tm.tm_sec;


    auto duration = now.time_since_epoch();
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
}


void timeToAngles(int hours, int minutes, int seconds, float milliseconds,
    float& hourAngle, float& minuteAngle, float& secondAngle)
{

    hours = hours % 12;


    secondAngle = radians((seconds + milliseconds / 1000.0f) * 6.0f);


    minuteAngle = radians((minutes + seconds / 60.0f) * 6.0f);


    hourAngle = radians((hours + minutes / 60.0f) * 30.0f);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.02f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        mat4 rotationMatrix = glm::rotate(mat4(1.0f), 0.01f, vec3(0.0f, 1.0f, 0.0f));
        cameraFront = vec3(rotationMatrix * vec4(cameraFront, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        mat4 rotationMatrix = glm::rotate(mat4(1.0f), -0.01f, vec3(0.0f, 1.0f, 0.0f));
        cameraFront = vec3(rotationMatrix * vec4(cameraFront, 0.0f));
    }
}

std::vector<vec3> drawSphere(vec3 positions, float radius) {
    std::vector<glm::vec3> vertices;
    int stacks = 20; // Reduced for better performance
    int slices = 20;
    for (int i = 0; i <= stacks; ++i) {
        float V = (float)i / (float)stacks;
        float phi = V * glm::pi<float>();

        for (int j = 0; j <= slices; ++j) {
            float U = (float)j / (float)slices;
            float theta = U * (glm::pi<float>() * 2);

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);vertices.push_back(glm::vec3(x, y, z) * radius + positions);
        }
    }
    return vertices;
}
std::vector<vec3> numOne(float rad) {
    std::vector<vec3> NUM_1;
    NUM_1.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04 + cos(radians(rad)) / 2.8f + 1, 0.001));
    NUM_1.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    return NUM_1;
}
std::vector<vec3> numTwo(float rad) {
    std::vector<vec3> arr;

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04 + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    return arr;
}
std::vector<vec3> numThree(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04 + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    return arr;

}
std::vector<vec3> numFour(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.03 + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.01f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.01f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.01f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.02 + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.03f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.01 + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    return arr;

}
std::vector<vec3> numFive(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));


    return arr;

}
std::vector<vec3> numSix(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001));



    return arr;

}
std::vector<vec3> numSeven(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.0f + sin(radians(rad)) / 2.8f, 0.02f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.0 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.0 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.045f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.045f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.025 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    return arr;

}
std::vector<vec3> numEight(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));



    return arr;

}std::vector<vec3> numNine(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, 0.04f + cos(radians(rad)) / 2.8f + 1, 0.001));



    return arr;

}
std::vector<vec3> numZero(float rad) {
    std::vector<vec3> arr;
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.005f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.005f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(-0.015 + sin(radians(rad)) / 2.8f, 0.005f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.075f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(-0.015f + sin(radians(rad)) / 2.8f, -0.075f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.075f + cos(radians(rad)) / 2.8f + 1, 0.001));

    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, -0.075f + cos(radians(rad)) / 2.8f + 1, 0.001));
    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));

    arr.push_back(vec3(0.015f + sin(radians(rad)) / 2.8f, 0.0f + cos(radians(rad)) / 2.8f + 1, 0.001f));
    arr.push_back(vec3(0.015 + sin(radians(rad)) / 2.8f, 0.005f + cos(radians(rad)) / 2.8f + 1, 0.001));



    return arr;

}

int main()
{

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "Real Time Wooden Clock", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");


    vec3 darkWood = vec3(0.35f, 0.16f, 0.07f);  // rich dark brown
    vec3 midWood = vec3(0.55f, 0.27f, 0.07f);   // mid tone
    vec3 lightWood = vec3(0.76f, 0.60f, 0.42f); // golden highlight
    vec3 gold = vec3(0.9f, 0.8f, 0.4f);         // metallic accent




    std::vector<PolygonLine> N;
    N.push_back(PolygonLine(numOne(0), vec3(0.0f, 0.0f, 0.0f)));
    N.push_back(PolygonLine(numTwo(3), vec3(0.0f, 0.0f, 0.0f)));

    N.push_back(PolygonLine(numOne(30), vec3(0.0f, 0.0f, 0.0f)));
    
    N.push_back(PolygonLine(numTwo(60), vec3(0.0f, 0.0f, 0.0f)));
  
    N.push_back(PolygonLine(numThree(90), vec3(0.0f, 0.0f, 0.0f)));
    
    N.push_back(PolygonLine(numFour(120), vec3(0.0f, 0.0f, 0.0f)));

    N.push_back(PolygonLine(numFive(150), vec3(0.0f, 0.0f, 0.0f)));

    N.push_back(PolygonLine(numSix(180), vec3(0.0f, 0.0f, 0.0f)));
    
    N.push_back(PolygonLine(numSeven(210), vec3(0.0f, 0.0f, 0.0f)));
    
    N.push_back(PolygonLine(numEight(240), vec3(0.0f, 0.0f, 0.0f)));

    N.push_back(PolygonLine(numNine(270), vec3(0.0f, 0.0f, 0.0f)));

    N.push_back(PolygonLine(numZero(307), vec3(0.0f, 0.0f, 0.0f)));
    N.push_back(PolygonLine(numOne(300), vec3(0.0f, 0.0f, 0.0f)));
    PolygonLine numOneTranslated = PolygonLine(numOne(330), vec3(0.0f, 0.0f, 0.0f));
    mat4 transformOne = mat4(1.0f);
    transformOne = translate(transformOne, vec3(0.03, 0, 0));
    numOneTranslated.transformation(transformOne);
    N.push_back(PolygonLine(numOne(330), vec3(0.0f, 0.0f, 0.0f)));
    N.push_back(numOneTranslated);
    

    std::vector<vec3> frontFace = {
        vec3(-0.6f, 1.6f, -0.001f),
        vec3(-0.3f, 0.0f, -0.001f),
        vec3(0.3f, 0.0f, -0.001f),
        vec3(0.6f, 1.6f, -0.001f) };
    Polygon Polygon1(frontFace, darkWood);

    std::vector<vec3> backFace = {
        vec3(-0.6f, 1.6f, -0.2f),
        vec3(-0.3f, 0.0f, -0.2f),
        vec3(0.3f, 0.0f, -0.2f),
        vec3(0.6f, 1.6f, -0.2f) };
    Polygon Polygon3(backFace, darkWood);
    // Various wood tones for a natural look
    vec3 roofWood = vec3(0.82f, 0.71f, 0.55f);      // Light oak
    vec3 groundWood = vec3(0.58f, 0.45f, 0.32f);    // Medium walnut  
    vec3 rightSideWood = vec3(0.70f, 0.55f, 0.40f); // Cherry wood
    vec3 leftSideWood = vec3(0.45f, 0.35f, 0.25f);  // Dark oak
    vec3 backWood = vec3(0.65f, 0.50f, 0.38f);      // Mahogany
    std::vector<vec3> vertices15 = {};  // Back Big RECTANGLE 
    vertices15.push_back(vec3(-1.2f, 2.0f, -1.0f)); // Top LEFT
    vertices15.push_back(vec3(-1.2f, -1.0f, -1.0f)); // Down LEFT
    vertices15.push_back(vec3(1.2f, -1.0f, -1.0)); // Down Right
    vertices15.push_back(vec3(1.2f, 2.0f, -1.0)); // Top Right
    Polygon Polygon15 = Polygon(vertices15, vec3(0.5196f, 0.4765f, 0.4059f));

    std::vector<vec3> vertices16 = {};  // Left Side Big RECTANGLE 
    vertices16.push_back(vec3(-1.2f, 2.0f, -1.0f)); // Top LEFT
    vertices16.push_back(vec3(-1.2f, -1.0f, -1.0f)); // Down LEFT
    vertices16.push_back(vec3(-1.2f, -1.0f, 1.0)); // Down Right
    vertices16.push_back(vec3(-1.2f, 2.0f, 1.0)); // Top Right
    Polygon Polygon16 = Polygon(vertices16, vec3(0.6696f, 0.6365f, 0.5859f));

    std::vector<vec3> vertices17 = {};  // Right side Big RECTANGLE 
    vertices17.push_back(vec3(1.2f, -1.0f, -1.0)); // Down Right
    vertices17.push_back(vec3(1.2f, 2.0f, -1.0)); // Top Right
    vertices17.push_back(vec3(1.2f, 2.0f, 1.0f)); // Top LEFT
    vertices17.push_back(vec3(1.2f, -1.0f, 1.0f)); // Down LEFT
    Polygon Polygon17 = Polygon(vertices17, vec3(0.8696f, 0.8265f, 0.7559f));

    std::vector<vec3> vertices18 = {};  // Down side Big RECTANGLE 
    vertices18.push_back(vec3(-1.2f, -1.0f, -1.0f)); // Down LEFT Back
    vertices18.push_back(vec3(-1.2f, -1.0f, 1.0)); // Down Left Frontv
    vertices18.push_back(vec3(1.2f, -1.0f, 1.0f)); // Down Right front   
    vertices18.push_back(vec3(1.2f, -1.0f, -1.0)); // Down Right back
    Polygon Polygon18 = Polygon(vertices18, vec3(0.6196f, 0.5765f, 0.5059f));

    std::vector<vec3> vertices19 = {};  // Up side Big RECTANGLE 
    vertices19.push_back(vec3(-1.2f, 2.0f, -1.0f)); // back Top LEFT
    vertices19.push_back(vec3(-1.2f, 2.0f, 1.0)); // front Top Left
    vertices19.push_back(vec3(1.2f, 2.0f, 1.0f)); // front Top Right
    vertices19.push_back(vec3(1.2f, 2.0f, -1.0)); // back Top Right
    Polygon Polygon19 = Polygon(vertices19, vec3(0.9196f, 0.8565f, 0.7559f));
    Polygon15 = Polygon(vertices15, backWood);        // Back wall
    Polygon16 = Polygon(vertices16, leftSideWood);    // Left side wall (shadow)
    Polygon17 = Polygon(vertices17, rightSideWood);   // Right side wall (light)
    Polygon18 = Polygon(vertices18, groundWood);      // Floor
    Polygon19 = Polygon(vertices19, roofWood);        // Ceiling/Roof
    vec3 doorFrameColor = vec3(0.45f, 0.35f, 0.25f);  // Dark oak for frame

    std::vector<vec3> vertices20 = {};  // Front Up Door  
    vertices20.push_back(vec3(-1.2f, 2.0f, 1.0)); // front Top Left
    vertices20.push_back(vec3(-1.2f, 1.8f, 1.0)); // front down Left
    vertices20.push_back(vec3(1.2f, 1.8f, 1.0f)); // front dowm Right
    vertices20.push_back(vec3(1.2f, 2.0f, 1.0f)); // front Top Right
    Polygon Polygon20 = Polygon(vertices20, doorFrameColor);  // Dark frame

    std::vector<vec3> vertices21 = {};  // lefr side Door  
    vertices21.push_back(vec3(-1.2f, 2.0f, 1.0)); //  Top Left
    vertices21.push_back(vec3(-1.2f, -1.0f, 1.0)); // Down left
    vertices21.push_back(vec3(-1.0f, -1.0f, 1.0)); // Down Right
    vertices21.push_back(vec3(-1.0f, 2.0f, 1.0)); //  Top right
    Polygon Polygon21 = Polygon(vertices21, doorFrameColor);  // Dark frame

    std::vector<vec3> vertices22 = {};  // Front down Door  
    vertices22.push_back(vec3(-1.2f, -0.8f, 1.0)); // top lefr
    vertices22.push_back(vec3(-1.2f, -1.0f, 1.0)); // Down lefr
    vertices22.push_back(vec3(1.2f, -1.0f, 1.0f)); // Down right
    vertices22.push_back(vec3(1.2f, -0.8f, 1.0f)); // top right
    Polygon Polygon22 = Polygon(vertices22, doorFrameColor);  // Dark frame
    std::vector<vec3> vertices23 = {};  // right side Door  
    vertices23.push_back(vec3(1.0f, 2.0f, 1.0f)); // Top LEFT
    vertices23.push_back(vec3(1.0f, -1.0f, 1.0f)); // Down LEFT
    vertices23.push_back(vec3(1.2f, -1.0f, 1.0f)); // Down right
    vertices23.push_back(vec3(1.2f, 2.0f, 1.0f)); // Top right
    Polygon Polygon23 = Polygon(vertices23, doorFrameColor);  // Dark frame

    std::vector<vec3> vertices24 = {};  // back Circle
    for (float i = 0; i < 30; i++) {
        vertices24.push_back(vec3(cos(i) / 10, sin(i) / 10 + 1.0f, -0.201f)); // TOP LEFT
    }
    Polygon Polygon24 = Polygon(vertices24, vec3(0.0f, 0.0f, 0.0f));



    std::vector<Polygon> MarkCircle;

    for (int i = 0; i < 60; i++) {
        float angle = i * 6.0f; // 6 degrees per minute mark
        float x = sin(radians(angle)) / 2.2f; // Match clock face radius
        float y = cos(radians(angle)) / 2.2f + 1.0f; // Center at y=1.0

        // Use consistent z-coordinate (slightly above clock face)
        float z = 0.002f;
        std::vector<vec3> markVertices;
        if (i % 5 == 0) { // Hour marks (every 5 minutes)
             markVertices = {
                vec3(x - 0.01f, y - 0.05f , z),
                vec3(x + 0.01f, y - 0.05f , z),
                vec3(x + 0.01f, y  , z),
                vec3(x - 0.01f, y , z)
            };
          
        }
        else { // Minute marks
           markVertices = {
                vec3(x-0.005, y - 0.03, z),
                vec3(x+0.005, y - 0.03f, z),
                vec3(x+0.005, y , z),
                vec3(x-0.005, y , z)
            };
        }
        Polygon mark(markVertices, vec3(0.0f, 0.0f, 0.0f)); // Black for hour marks

            mat4 trans = mat4(1.0);
            trans = translate(trans, vec3(x, y , z));
            trans = rotate(trans,-radians((float)angle), vec3(0, 0, 1));
            trans = translate(trans, vec3(-(x), -(y), z));
            mark.transformation(trans);
            MarkCircle.push_back(mark);
            
    }

    //std::vector<PolygonLine> MarkCircle = {};  // mark Circle
    //for (float i = 0, j = 0; i < 12, j <= 360; i++, j += 30) {
    //    float x = cos(radians(j)) / 2.8f;
    //    float y = sin(radians(j)) / 2.8f + 1;
    //    std::vector<vec3> vertices25;
    //    vertices25.push_back(vec3(-0.015 + x, 0.04 + y, 0.001));
    //    vertices25.push_back(vec3(0.015f + x, 0.04f + y, 0.001f));
    //    vertices25.push_back(vec3(0.015f+ x, 0.04f + y, 0.001f));
    //    vertices25.push_back(vec3(0.015f+ x, 0.0f + y, 0.001f));
    //    vertices25.push_back(vec3(0.015f + x, 0.0f + y, 0.001f));
    //    vertices25.push_back(vec3(-0.015f + x, 0.0f + y, 0.001f));
    //    vertices25.push_back(vec3(-0.015f +x, 0.0f + y, 0.001f));
    //    vertices25.push_back(vec3(-0.015f + x, -0.04f + y, 0.001f));
    //    vertices25.push_back(vec3(-0.015f + x, -0.04f + y, 0.001f));
    //    vertices25.push_back(vec3(0.015f+ x, -0.04f + y, 0.001f));
    //    PolygonLine mark(vertices25, vec3(0.0f, 0.0f, 0.0f));

    //    MarkCircle.push_back(mark);
    //    //drawNumber(i, ourShader, x, y, 0.15f);
    //}
    std::vector<vec3> leftFace = {
        vec3(-0.6f, 1.6f, -0.2f),
        vec3(-0.3f, 0.0f, -0.2f),
        vec3(-0.3f, 0.0f, -0.001f),
        vec3(-0.6f, 1.6f, -0.001f) };
    Polygon Polygon4(leftFace, midWood);

    std::vector<vec3> rightFace = {
        vec3(0.6f, 1.6f, -0.2f),
        vec3(0.6f, 1.6f, -0.001f),
        vec3(0.3f, 0.0f, -0.001f),
        vec3(0.3f, 0.0f, -0.2f) };
    Polygon Polygon5(rightFace, midWood);

    std::vector<vec3> bottomFace = {
        vec3(-0.3f, 0.0f, -0.2f),
        vec3(-0.3f, 0.0f, -0.001f),
        vec3(0.3f, 0.0f, -0.001f),
        vec3(0.3f, 0.0f, -0.2f) };
    Polygon Polygon6(bottomFace, darkWood);

    std::vector<vec3> topFace = {
        vec3(-0.6f, 1.6f, -0.2f),
        vec3(-0.6f, 1.6f, -0.001f),
        vec3(0.6f, 1.6f, -0.001f),
        vec3(0.6f, 1.6f, -0.2f) };
    Polygon Polygon7(topFace, darkWood);// ------------------------------------------------------------
    // CLOCK FACE (dial)
    // ------------------------------------------------------------
    std::vector<vec3> circleVerts;
    for (float i = 0; i < 400; i++)
    {
        circleVerts.push_back(vec3(cos(i) / 2.2f, sin(i) / 2.2f + 1.0f, 0.0f));
    }
    Polygon ClockFace(circleVerts, vec3(0.95f, 0.93f, 0.88f)); // ivory face

    // ------------------------------------------------------------
    // CLOCK HANDS
    // ------------------------------------------------------------
    std::vector<vec3> secVerts = {
        vec3(-0.005, 1.0f, 0.001f),
        vec3(-0.005f, (1 / 2.2f - 0.1f) + 1.0f, 0.001f),
        vec3(0.005f, (1 / 2.2f - 0.1f) + 1.0f, 0.001f),
        vec3(0.005f, 1.0f, 0.001f) };

    std::vector<vec3> minVerts = {
        vec3(-0.01f, 1.0f, 0.001f),
        vec3(-0.01f, (1 / 2.2f - 0.15f) + 1.0f, 0.001f),
        vec3(0.01f, (1 / 2.2f - 0.15f) + 1.0f, 0.0001f),
        vec3(0.01f, 1.0f, 0.0001f) };

    std::vector<vec3> hourVerts = {
        vec3(-0.012f, 1.0f, 0.001f),
        vec3(-0.012f, (1 / 2.2f - 0.25f) + 1.0f, 0.00001f),
        vec3(0.012f, (1 / 2.2f - 0.25f) + 1.0f, 0.00001f),
        vec3(0.012f, 1.0f, 0.001f) };
    Polygon secondHand(secVerts, vec3(1, 0, 0));  // Red second hand
    Polygon minuteHand(minVerts, vec3(0, 0, 0));  // Black minute hand                    
    Polygon hourHand(hourVerts, vec3(0, 0, 0));   // Black hour hand

    // ------------------------------------------------------------
    // GLASS COVER
    // ------------------------------------------------------------
    std::vector<vec3> glassVerts;
    for (float i = 0; i < 400; i++)
    {
        glassVerts.push_back(vec3(cos(i) / 2.2f, sin(i) / 2.2f + 1.0f, 0.01f));
    }
    Polygon GlassPolygon(glassVerts, vec3(1.0f, 1.0f, 1.0f));

    // ------------------------------------------------------------
    // DANCER/PENDULUM
    // ------------------------------------------------------------
    vec3 positions(0.0f, -0.8f, -0.1f);
    vec3 dancerSphereColor = vec3(0.58f, 0.45f, 0.32f); // Walnut
    float radius = 0.1;
    Polygon DancerCirclelePolygon = Polygon(drawSphere(positions, radius), midWood);
    vec3 dancerBodyColor = vec3(0,0,0);  // Cherry wood
    std::vector<vec3> dancerBodyVertsFrontFace = {
        // Top (wider)
        vec3(-0.07f, 0.2f, -0.075f),
        vec3(0.07f, 0.2f, -0.075f),

        // Middle
        vec3(-0.05f, 0.0f, -0.075f),
        vec3(0.05f, 0.0f, -0.075f),

        // Bottom (narrower)
        vec3(-0.03f, -0.8f, -0.075f),
        vec3(0.03f, -0.8f, -0.075f)
    };
    Polygon DancerRectanglePolygonFrontFace(dancerBodyVertsFrontFace, dancerBodyColor);

    std::vector<vec3> dancerBodyVertsBackFace = {
    vec3(-0.07f, 0.2f, -0.15f),
    vec3(0.07f, 0.2f, -0.15f),
    vec3(-0.05f, 0.0f, -0.15f),
    vec3(0.05f, 0.0f, -0.15f),
    vec3(-0.03f, -0.8f, -0.15f),
    vec3(0.03f, -0.8f, -0.15f)
    };
    Polygon DancerRectanglePolygonBackFace(dancerBodyVertsBackFace, dancerBodyColor);

    // ------------------------------------------------------------
    // SHADER CONFIGURATION
    // ------------------------------------------------------------
    ourShader.use();
    mat4 projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // ------------------------------------------------------------
    // RENDER LOOP
    // ------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // Draw wooden body
        Polygon1.draw(ourShader);
        Polygon3.draw(ourShader);
        Polygon4.draw(ourShader);
        Polygon5.draw(ourShader);
        Polygon6.draw(ourShader);
        Polygon7.draw(ourShader);
        // back circle
        mat4 transformation4 = mat4(1.0f);transformation4 = translate(transformation4, glm::vec3(0.0f, 1.0f, 0.0f));
        transformation4 = rotate(transformation4, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, -1.0f));
        transformation4 = translate(transformation4, glm::vec3(0.0f, -1.0f, 0.0f));

        Polygon24.transformation(transformation4);
        Polygon24.draw(ourShader);

        Polygon15.draw(ourShader);
        Polygon16.draw(ourShader);
        Polygon17.draw(ourShader);
        Polygon18.draw(ourShader);
        Polygon19.draw(ourShader);
        Polygon20.draw(ourShader);
        Polygon21.draw(ourShader);
        Polygon22.draw(ourShader);
        Polygon23.draw(ourShader);
        Polygon24.draw(ourShader);
        for (auto it : N) {
            it.draw(ourShader);
        }
        for (auto mark : MarkCircle)
        {
            mark.draw(ourShader);
        }
        // Get REAL system time
        int hours, minutes, seconds;
        float milliseconds;
        getCurrentTime(hours, minutes, seconds, milliseconds);

        // Convert to rotation angles
        float hourAngle, minuteAngle, secondAngle;
        timeToAngles(hours, minutes, seconds, milliseconds,
            hourAngle, minuteAngle, secondAngle);

        // Draw dancer with real-time synchronized swinging
        mat4 dancerTrans = mat4(1.0f);

        float swingAngle = 0.2f * sin(4.8 * glfwGetTime());
        dancerTrans = rotate(dancerTrans, swingAngle, vec3(0.0f, 0.0f, -1.0f));
        DancerCirclelePolygon.transformation(dancerTrans);
        DancerRectanglePolygonFrontFace.transformation(dancerTrans);
        DancerRectanglePolygonBackFace.transformation(dancerTrans);
        DancerCirclelePolygon.draw(ourShader);
        DancerRectanglePolygonFrontFace.draw(ourShader);
        DancerRectanglePolygonBackFace.draw(ourShader);

        // Draw clock face
        ClockFace.draw(ourShader);

        // REAL TIME Clock hands using system time
        // Seconds hand
        mat4 secTrans = mat4(1.0f);
        secTrans = translate(secTrans, vec3(0.0f, 1.0f, 0.0f));
        secTrans = rotate(secTrans, secondAngle, vec3(0.0f, 0.0f, -1.0f));
        secTrans = translate(secTrans, vec3(0.0f, -1.0f, 0.0f));
        secondHand.transformation(secTrans);
        secondHand.draw(ourShader);

        // Minutes hand
        mat4 minTrans = mat4(1.0f);
        minTrans = translate(minTrans, vec3(0.0f, 1.0f, 0.0f));
        minTrans = rotate(minTrans, minuteAngle, vec3(0.0f, 0.0f, -1.0f));
        minTrans = translate(minTrans, vec3(0.0f, -1.0f, 0.0f));
        minuteHand.transformation(minTrans);
        minuteHand.draw(ourShader);

        // Hours hand
        mat4 hourTrans = mat4(1.0f);
        hourTrans = translate(hourTrans, vec3(0.0f, 1.0f, 0.0f));
        hourTrans = rotate(hourTrans, hourAngle, vec3(0.0f, 0.0f, -1.0f));
        hourTrans = translate(hourTrans, vec3(0.0f, -1.0f, 0.0f));
        hourHand.transformation(hourTrans);
        hourHand.draw(ourShader);

        // Draw the glass cover last (blending enabled)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ourShader.setFloat("alpha", 0.1f);
        GlassPolygon.draw(ourShader);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}