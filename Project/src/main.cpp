#include "glad.h"
#include "glfw3.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Game *gamePtr = nullptr;

int main()
{
    // seed random
    srand((unsigned int)time(NULL));

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          "Geometry Runner", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure OpenGL
    // ----------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Game game((float)SCR_WIDTH, (float)SCR_HEIGHT);
    gamePtr = &game;
    game.init();

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        // input
        glfwPollEvents();
        game.processInput(deltaTime);

        // update
        game.update(deltaTime);

        // render
        glClearColor(0.01f, 0.01f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game.render(currentFrame);

        // swap
        glfwSwapBuffers(window);
    }

    gamePtr = nullptr;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// keyboard callback
// -----------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            if (gamePtr) gamePtr->keys[key] = true;
            if (gamePtr) gamePtr->keysProcessed[key] = false;
        }
        else if (action == GLFW_RELEASE)
        {
            if (gamePtr) gamePtr->keys[key] = false;
            if (gamePtr) gamePtr->keysProcessed[key] = false;
        }
    }
}

// framebuffer resize callback
// ---------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
