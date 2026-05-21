#include "game.h"
#include <cmath>
#include <cstdio>
#include <iostream>

static const float GROUND_Y_POS = 100.0f;


// Constructor

Game::Game(float w, float h)
    : state(GAME_MENU), score(0), highScore(0),
      screenWidth(w), screenHeight(h),
      shapeShader(nullptr), backgroundShader(nullptr),
      renderer(nullptr),
      groundY(GROUND_Y_POS),
      gameOverTimer(0.0f),
      totalDistance(0.0f)
{
    for (int i = 0; i < 1024; i++)
    {
        keys[i] = false;
        keysProcessed[i] = false;
    }
}

Game::~Game()
{
    delete shapeShader;
    delete backgroundShader;
    delete renderer;
}


// Initialize

void Game::init()
{
    shapeShader      = new Shader("resources/shaders/shape.vs",
                                  "resources/shaders/shape.fs");
    backgroundShader = new Shader("resources/shaders/background.vs",
                                  "resources/shaders/background.fs");

    renderer = new Renderer();
    renderer->initRenderData();

    player.reset(150.0f, groundY);
    obstacleManager.reset();
}


// Process input

void Game::processInput(float dt)
{
    if (state == GAME_MENU)
    {
        if (keys[GLFW_KEY_SPACE] && !keysProcessed[GLFW_KEY_SPACE])
        {
            state = GAME_ACTIVE;
            reset();
            keysProcessed[GLFW_KEY_SPACE] = true;
        }
    }
    else if (state == GAME_ACTIVE)
    {
        // jump with space, up arrow, or W
        if (keys[GLFW_KEY_SPACE] || keys[GLFW_KEY_UP] || keys[GLFW_KEY_W])
        {
            player.jump();
        }

        if (keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE])
        {
            state = GAME_MENU;
            keysProcessed[GLFW_KEY_ESCAPE] = true;
        }
    }
    else if (state == GAME_OVER)
    {
        if (keys[GLFW_KEY_SPACE] && !keysProcessed[GLFW_KEY_SPACE])
        {
            state = GAME_ACTIVE;
            reset();
            keysProcessed[GLFW_KEY_SPACE] = true;
        }
        if (keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE])
        {
            state = GAME_MENU;
            keysProcessed[GLFW_KEY_ESCAPE] = true;
        }
    }
}


// Update

void Game::update(float dt)
{
    if (state != GAME_ACTIVE) return;

    // update player
    player.update(dt, groundY);

    // update obstacles
    obstacleManager.update(dt, screenWidth);

    // collision check
    if (obstacleManager.checkCollision(player.pos, player.size))
    {
        player.isAlive = false;
        state = GAME_OVER;
        gameOverTimer = 0.0f;

        if (score > highScore) highScore = score;
        return;
    }

    // update score (based on distance)
    totalDistance += obstacleManager.scrollSpeed * dt;
    score = (int)(totalDistance / 50.0f);
}


// Render

void Game::render(float time)
{
    Mat4 proj = getProjection();

    // animated background
    backgroundShader->use();
    backgroundShader->setFloat("time", time);
    backgroundShader->setFloat("scrollX", totalDistance);
    backgroundShader->setVec2("resolution", screenWidth, screenHeight);
    renderer->drawBackground(*backgroundShader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (state == GAME_MENU)
    {
        renderGround(time);
        renderMenu(time);
    }
    else if (state == GAME_ACTIVE)
    {
        renderGround(time);
        obstacleManager.render(*renderer, *shapeShader, proj, groundY, time);
        player.render(*renderer, *shapeShader, proj, time);
        renderHUD(time);
    }
    else if (state == GAME_OVER)
    {
        gameOverTimer += 0.016f;
        renderGround(time);
        obstacleManager.render(*renderer, *shapeShader, proj, groundY, time);
        renderGameOver(time);
    }
}


// Reset

void Game::reset()
{
    player.reset(150.0f, groundY);
    obstacleManager.reset();
    score = 0;
    totalDistance = 0.0f;
    gameOverTimer = 0.0f;
}


// Projection

Mat4 Game::getProjection() const
{
    return Mat4::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
}


// Render ground line + surface

void Game::renderGround(float time)
{
    Mat4 proj = getProjection();

    // ground surface (darker blue)
    renderer->drawQuad(0.0f, 0.0f, screenWidth, groundY,
                       0.02f, 0.02f, 0.02f, 1.0f,
                       *shapeShader, proj);

    // ground line (static cyan)
    renderer->drawQuad(0.0f, groundY, screenWidth, 3.0f,
                       0.2f, 0.2f, 0.2f, 1.0f,
                       *shapeShader, proj);

    // static dotted pattern on ground surface
    float dotSpacing = 25.0f;
    float dotSize = 2.0f;
    for (float dx = 0.0f; dx < screenWidth; dx += dotSpacing)
    {
        for (float dy = 10.0f; dy < groundY - 5.0f; dy += dotSpacing)
        {
            renderer->drawQuad(dx, dy, dotSize, dotSize,
                               0.0f, 0.3f, 0.5f, 0.3f,
                               *shapeShader, proj);
        }
    }
}


// Render main menu

void Game::renderMenu(float time)
{
    Mat4 proj = getProjection();

    // title
    renderer->drawText("TRIANGULAR JUMP",
                       screenWidth * 0.5f - 210.0f,
                       screenHeight * 0.7f, 6.0f,
                       0.0f, 1.0f, 1.0f, 1.0f,
                       *shapeShader, proj);

    // static red triangle in menu
    renderer->drawRightTriangle(
        screenWidth * 0.5f, screenHeight * 0.48f, 45.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.2f, 0.2f, 1.0f,
        *shapeShader, proj);

    // prompt
    renderer->drawText("PRESS SPACE TO START",
                       screenWidth * 0.5f - 180.0f,
                       screenHeight * 0.25f, 3.0f,
                       0.8f, 0.8f, 0.9f, 1.0f,
                       *shapeShader, proj);


    renderer->drawText("PRESS X TO EXIT",
                       screenWidth * 0.5f - 110.0f,
                       screenHeight * 0.05f, 2.0f,
                       0.4f, 0.4f, 0.5f, 0.7f,
                       *shapeShader, proj);

    if (highScore > 0)
    {
        char hs[64];
        snprintf(hs, sizeof(hs), "BEST: %d", highScore);
        renderer->drawText(hs, screenWidth * 0.5f - 55.0f,
                           screenHeight * 0.15f, 2.5f,
                           0.5f, 0.5f, 0.7f, 0.8f,
                           *shapeShader, proj);
    }
}


// HUD

void Game::renderHUD(float time)
{
    Mat4 proj = getProjection();

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    renderer->drawText(scoreText,
                       screenWidth * 0.5f - 20.0f,
                       screenHeight - 35.0f, 5.0f,
                       1.0f, 1.0f, 1.0f, 0.9f,
                       *shapeShader, proj);

    // speed indicator
    int speedPct = (int)(obstacleManager.difficulty * 100.0f);
    char spdText[32];
    snprintf(spdText, sizeof(spdText), "SPD:%d", speedPct);
    renderer->drawText(spdText, 15.0f, screenHeight - 25.0f, 2.0f,
                       0.4f, 0.7f, 0.9f, 0.6f,
                       *shapeShader, proj);
}


// Game over screen

void Game::renderGameOver(float time)
{
    Mat4 proj = getProjection();

    // overlay
    float overlayA = clampf(gameOverTimer * 3.0f, 0.0f, 0.6f);
    renderer->drawQuad(0, 0, screenWidth, screenHeight,
                       0.0f, 0.0f, 0.0f, overlayA,
                       *shapeShader, proj);

    if (gameOverTimer > 0.3f)
    {
        renderer->drawText("GAME OVER",
                           screenWidth * 0.5f - 120.0f,
                           screenHeight * 0.65f, 5.5f,
                           1.0f, 0.2f, 0.2f, 1.0f,
                           *shapeShader, proj);

        char st[64];
        snprintf(st, sizeof(st), "SCORE: %d", score);
        renderer->drawText(st, screenWidth * 0.5f - 100.0f,
                           screenHeight * 0.50f, 3.5f,
                           1.0f, 1.0f, 1.0f, 1.0f,
                           *shapeShader, proj);

        char hs[64];
        snprintf(hs, sizeof(hs), "BEST: %d", highScore);
        renderer->drawText(hs, screenWidth * 0.5f - 70.0f,
                           screenHeight * 0.42f, 3.0f,
                           0.0f, 1.0f, 0.7f, 0.9f,
                           *shapeShader, proj);

        renderer->drawText("PRESS SPACE TO RESTART",
                           screenWidth * 0.5f - 180.0f,
                           screenHeight * 0.28f, 3.0f,
                           0.8f, 0.8f, 0.8f, 1.0f,
                           *shapeShader, proj);

        renderer->drawText("PRESS X TO EXIT",
                           screenWidth * 0.5f - 110.0f,
                           screenHeight * 0.20f, 2.0f,
                           0.4f, 0.4f, 0.5f, 0.7f,
                           *shapeShader, proj);
    }
}


