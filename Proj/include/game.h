#ifndef GAME_H
#define GAME_H

#include "glad.h"
#include "glfw3.h"
#include "math_utils.h"
#include "shader.h"
#include "renderer.h"
#include "player.h"
#include "obstacle.h"

enum GameState { GAME_MENU, GAME_ACTIVE, GAME_OVER };

class Game
{
public:
    GameState state;
    int  score;
    int  highScore;
    bool keys[1024];
    bool keysProcessed[1024];
    float screenWidth, screenHeight;

    Game(float w, float h);
    ~Game();

    void init();
    void processInput(float dt);
    void update(float dt);
    void render(float time);
    void reset();

private:
    Shader *shapeShader, *backgroundShader;
    Renderer *renderer;
    Player player;
    ObstacleManager obstacleManager;

    float groundY;
    float gameOverTimer;
    float totalDistance;

    Mat4 getProjection() const;
    void renderGround(float time);
    void renderMenu(float time);
    void renderHUD(float time);
    void renderGameOver(float time);

};

#endif
