#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float centerX = -0.7469;
float centerY = 0.1102;
float zoom = 1.0;
float zoomSpeed = 1.02;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fractal Zoom", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("5.1.transform.vs", "5.1.transform.fs");
    if (shader.ID == 0) {
        std::cerr << "CRITICAL ERROR: Shader program ID is 0. Check console for details." << std::endl;
        return -1;
    }

    float vertices[] = {
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "maxIter"), 100);

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        zoom *= pow(zoomSpeed, deltaTime * 2.0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glUniform1f(glGetUniformLocation(shader.ID, "centerX"), (float)centerX);
        glUniform1f(glGetUniformLocation(shader.ID, "centerY"), (float)centerY);
        glUniform1f(glGetUniformLocation(shader.ID, "zoom"), (float)zoom);
        glUniform1f(glGetUniformLocation(shader.ID, "time"), (float)currentTime);
        glUniform1i(glGetUniformLocation(shader.ID, "maxIter"), (int)(50 + sqrt(zoom) * 15));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        zoomSpeed = fmin(1.15, zoomSpeed + 0.01);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        zoomSpeed = fmax(1.001, zoomSpeed - 0.01);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        centerX -= 0.003 / zoom;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        centerX += 0.003 / zoom;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        centerY += 0.003 / zoom;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        centerY -= 0.003 / zoom;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}