#include "glad.c"      // inline OpenGL loader — no separate glad.c needed
#include "glfw3.h"
#include <stdio.h>

// ── Shaders ───────────────────────────────────────────────────────────────────
const char* VS =
    "#version 330 core\n"
    "layout(location=0) in vec2 pos;\n"
    "uniform vec2 offset;\n"
    "void main() { gl_Position = vec4(pos + offset, 0.0, 1.0); }\n";

const char* FS =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "uniform vec4 color;\n"
    "void main() { fragColor = color; }\n";

// ── Constants ─────────────────────────────────────────────────────────────────
#define MAX_OBS   4
#define GROUND_Y -0.65f
#define PLAYER_X -0.55f

// ── State ─────────────────────────────────────────────────────────────────────
float posY = 0.0f, velY = 0.0f;
int   onGround = 1, gameOver = 0, score = 0;
float speed = 0.55f, spawnTimer = 0.0f, spawnInterval = 1.8f;

typedef struct { float x; int active, scored; } Obs;
Obs obs[MAX_OBS];

void resetGame() {
    posY = 0.0f; velY = 0.0f; onGround = 1;
    gameOver = 0; score = 0;
    speed = 0.55f; spawnTimer = 0.0f; spawnInterval = 1.8f;
    for (int i = 0; i < MAX_OBS; i++) obs[i].active = 0;
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(700, 500, "Triangle Jump", NULL, NULL);
    glfwMakeContextCurrent(win);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Player triangle
    float triVerts[] = {
         0.0f,  0.12f,
        -0.08f, 0.0f,
         0.08f, 0.0f
    };
    unsigned int triVAO, triVBO;
    glGenVertexArrays(1, &triVAO); glGenBuffers(1, &triVBO);
    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Obstacle rectangle (two triangles)
    float obsVerts[] = {
        -0.035f, 0.0f,    0.035f, 0.0f,    0.035f, 0.30f,
        -0.035f, 0.0f,    0.035f, 0.30f,  -0.035f, 0.30f
    };
    unsigned int obsVAO, obsVBO;
    glGenVertexArrays(1, &obsVAO); glGenBuffers(1, &obsVBO);
    glBindVertexArray(obsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, obsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obsVerts), obsVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compile shaders
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VS, NULL); glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FS, NULL); glCompileShader(fs);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs); glDeleteShader(fs);

    int uOffset = glGetUniformLocation(prog, "offset");
    int uColor  = glGetUniformLocation(prog, "color");

    resetGame();
    double prevTime = glfwGetTime();
    int spaceWasDown = 0;
    char title[64];

    // ── Game loop ─────────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(win, 1);

        int spaceDown = (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS);

        double now = glfwGetTime();
        float  dt  = (float)(now - prevTime);
        prevTime   = now;

        if (gameOver) {
            if (spaceDown && !spaceWasDown) {
                resetGame();
                glfwSetWindowTitle(win, "Triangle Jump");
            }
        } else {
            // Jump
            if (spaceDown && !spaceWasDown && onGround) {
                velY = 2.5f; onGround = 0;
            }

            // Physics
            velY += -4.0f * dt;
            posY += velY  * dt;
            if (posY <= 0.0f) { posY = 0.0f; velY = 0.0f; onGround = 1; }

            // Spawn obstacle
            spawnTimer += dt;
            if (spawnTimer >= spawnInterval) {
                spawnTimer = 0.0f;
                for (int i = 0; i < MAX_OBS; i++) {
                    if (!obs[i].active) {
                        obs[i].x = 1.2f; obs[i].active = 1; obs[i].scored = 0;
                        break;
                    }
                }
            }

            // Move + score + collision
            for (int i = 0; i < MAX_OBS; i++) {
                if (!obs[i].active) continue;

                obs[i].x -= speed * dt;

                if (!obs[i].scored && obs[i].x < PLAYER_X - 0.08f) {
                    obs[i].scored = 1; score++;
                    if (score % 5 == 0) {
                        speed += 0.08f;
                        if (spawnInterval > 1.0f) spawnInterval -= 0.15f;
                    }
                    snprintf(title, sizeof(title), "Triangle Jump  |  Score: %d", score);
                    glfwSetWindowTitle(win, title);
                }

                if (obs[i].x < -1.3f) { obs[i].active = 0; continue; }

                // AABB collision
                float px0 = PLAYER_X - 0.06f, px1 = PLAYER_X + 0.06f;
                float py0 = GROUND_Y + posY,   py1 = py0 + 0.10f;
                float ox0 = obs[i].x - 0.028f, ox1 = obs[i].x + 0.028f;
                float oy0 = GROUND_Y,           oy1 = oy0 + 0.30f;

                if (px1 > ox0 && px0 < ox1 && py1 > oy0 && py0 < oy1) {
                    gameOver = 1;
                    snprintf(title, sizeof(title),
                             "GAME OVER  |  Score: %d  |  SPACE to restart", score);
                    glfwSetWindowTitle(win, title);
                }
            }
        }

        spaceWasDown = spaceDown;

        // ── Render ────────────────────────────────────────────────────────────
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);

        // Player
        glUniform2f(uOffset, PLAYER_X, GROUND_Y + posY);
        glUniform4f(uColor, gameOver ? 0.9f : 1.0f,
                            gameOver ? 0.25f : 0.85f,
                            0.2f, 1.0f);
        glBindVertexArray(triVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Obstacles
        glUniform4f(uColor, 0.3f, 0.85f, 1.0f, 1.0f);
        glBindVertexArray(obsVAO);
        for (int i = 0; i < MAX_OBS; i++) {
            if (!obs[i].active) continue;
            glUniform2f(uOffset, obs[i].x, GROUND_Y);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(win);
    }

    glfwTerminate();
    return 0;
}
