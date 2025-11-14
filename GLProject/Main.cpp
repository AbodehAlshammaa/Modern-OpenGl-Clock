// GLProject.cpp : 3D Wooden Clock with Glass Cover
// -------------------------------------------------

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon.cpp"
using namespace glm;

int width = 1920;
int height = 1080;

vec3 cameraPos = vec3(0.0f, 0.0f, 4.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

// ------------------------------------------------------------
// Handle keyboard input for camera movement
// ------------------------------------------------------------
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
std::vector<vec3> DrawSphere(vec3 positions, float radius) {
    std::vector<glm::vec3> vertices;
    int stacks = 30; // Number of subdivisions along the vertical axis
    int slices = 30; // Number of subdivisions along the horizontal axis
    for (int i = 0; i <= stacks; ++i) {
        float V = (float)i / (float)stacks;
        float phi = V * glm::pi<float>(); // Latitude angle

        for (int j = 0; j <= slices; ++j) {
            float U = (float)j / (float)slices;
            float theta = U * (glm::pi<float>() * 2); // Longitude angle

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            vertices.push_back(glm::vec3(x, y, z) * radius + positions);
            // Calculate normals and texture coordinates similarly
        }
    }
    return vertices;
}

// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------
int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "Wooden Clock", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

    // ------------------------------------------------------------
    // Define wood-tone and accent colors
    // ------------------------------------------------------------
    vec3 darkWood = vec3(0.35f, 0.16f, 0.07f);  // rich dark brown
    vec3 midWood = vec3(0.55f, 0.27f, 0.07f);   // mid tone
    vec3 lightWood = vec3(0.76f, 0.60f, 0.42f); // golden highlight
    vec3 gold = vec3(0.9f, 0.8f, 0.4f);         // metallic accent

    // ------------------------------------------------------------
    // CLOCK BODY (wooden case)
    // ------------------------------------------------------------
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
    Polygon Polygon7(topFace, darkWood);

    // ------------------------------------------------------------
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
        vec3(0.01f, (1 / 2.2f - 0.15f) + 1.0f, 0.001f),
        vec3(0.01f, 1.0f, 0.001f) };

    std::vector<vec3> hourVerts = {
        vec3(-0.012f, 1.0f, 0.001f),
        vec3(-0.012f, (1 / 2.2f - 0.25f) + 1.0f, 0.001f),
        vec3(0.012f, (1 / 2.2f - 0.25f) + 1.0f, 0.001f),
        vec3(0.012f, 1.0f, 0.001f) };
    Polygon secondHand(secVerts, vec3(0, 0, 0));
    Polygon minuteHand(minVerts, vec3(0, 0, 0));
    Polygon hourHand(hourVerts, vec3(0, 0, 0));

    // ------------------------------------------------------------
    // GLASS COVER
    // ------------------------------------------------------------

    std::vector<vec3> glassVerts;

    for (float i = 0; i < 400; i++)
    {
        glassVerts.push_back(vec3(cos(i) / 2.2f, sin(i) / 2.2f + 1.0f, 0.01f));
    }
    Polygon GlassPolygon(glassVerts, vec3(1.0f, 1.0f, 1.0f));


    // ----------------------
    // dancer
    // --------------------
    vec3 positions(0.0f, -0.8f, -0.1f);
    float radius = 0.2f;
    Polygon DancerCirclelePolygon = Polygon(DrawSphere(positions, radius), midWood);

   

    std::vector<vec3> dancerRectangleVertsFrontFace = {}; // front
    dancerRectangleVertsFrontFace.push_back(vec3(-0.1f, 0.2f, -0.075f)); // TOP LEFT
    dancerRectangleVertsFrontFace.push_back(vec3(-0.1f, -0.8f, -0.075f)); // DOWN LEFT
    dancerRectangleVertsFrontFace.push_back(vec3(0.1f, -0.8f, -0.075f)); // DOWN RIGHT
    dancerRectangleVertsFrontFace.push_back(vec3(0.1f, 0.2f, -0.075f));

    Polygon DancerRectanglePolygonFrontFace(dancerRectangleVertsFrontFace, vec3(1.0f, 1.0f, 0.0f));




    std::vector<vec3> dancerRectangleVertsBackFace = {}; // back
    dancerRectangleVertsBackFace.push_back(vec3(-0.1f, 0.2f, -0.15f)); // TOP LEFT
    dancerRectangleVertsBackFace.push_back(vec3(-0.1f, -0.8f, -0.15f)); // DOWN LEFT
    dancerRectangleVertsBackFace.push_back(vec3(0.1f, -0.8f, -0.15f)); // DOWN RIGHT
    dancerRectangleVertsBackFace.push_back(vec3(0.1f, 0.2f, -0.15f));
    Polygon DancerRectanglePolygonBackFace(dancerRectangleVertsBackFace, vec3(1.0f, 1.0f, 0.0f));

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

        // Time-based rotations for hands
        float t = glfwGetTime();
        //draw dancer 
        mat4 dancerTrans = mat4(1.0f);
        float swingAngle = 0.3f * sin(5.0f * glfwGetTime());
        dancerTrans = rotate(dancerTrans, swingAngle, vec3(0.0f, 0.0f, -1.0f));
        DancerCirclelePolygon.transformation(dancerTrans);
        DancerRectanglePolygonFrontFace.transformation(dancerTrans);
        DancerRectanglePolygonBackFace.transformation(dancerTrans);
        DancerCirclelePolygon.draw(ourShader);
        DancerRectanglePolygonFrontFace.draw(ourShader);
        DancerRectanglePolygonBackFace.draw(ourShader);


        // Draw clock face
        ClockFace.draw(ourShader);


        // Seconds
        mat4 secTrans = mat4(1.0f);
        secTrans = translate(secTrans, vec3(0.0f, 1.0f, 0.0f));
        secTrans = rotate(secTrans, 5.0f * t, vec3(0.0f, 0.0f, -1.0f));
        secTrans = translate(secTrans, vec3(0.0f, -1.0f, 0.0f));
        secondHand.transformation(secTrans);
        secondHand.draw(ourShader);

        // Minutes
        mat4 minTrans = mat4(1.0f);
        minTrans = translate(minTrans, vec3(0.0f, 1.0f, 0.0f));
        minTrans = rotate(minTrans, (5.0f * t) / 60.0f, vec3(0.0f, 0.0f, -1.0f));
        minTrans = translate(minTrans, vec3(0.0f, -1.0f, 0.0f));
        minuteHand.transformation(minTrans);
        minuteHand.draw(ourShader);

        // Hours
        mat4 hourTrans = mat4(1.0f);
        hourTrans = translate(hourTrans, vec3(0.0f, 1.0f, 0.0f));
        hourTrans = rotate(hourTrans, (5.0f * t) / 3600.0f, vec3(0.0f, 0.0f, -1.0f));
        hourTrans = translate(hourTrans, vec3(0.0f, -1.0f, 0.0f));
        hourHand.transformation(hourTrans);
        hourHand.draw(ourShader);
        //Draw the glass cover last (blending enabled)
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
