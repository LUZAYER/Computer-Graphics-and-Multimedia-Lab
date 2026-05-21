#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "math_utils.h"
#include "renderer.h"
#include "shader.h"

#include <vector>


// Obstacle types

enum ObstacleType
{
    OBS_BLOCK,         // tall rectangle
    OBS_SPIKE,         // triangle spike on ground
    OBS_DOUBLE_BLOCK,  // two blocks with gap
    OBS_LOW_BLOCK      // short block (duck or jump)
};


// Single obstacle

struct Obstacle
{
    float x, y;
    float width, height;
    ObstacleType type;
    bool active;

    // for spike type
    float spikeW, spikeH;
};


// Obstacle manager

class ObstacleManager
{
public:
    std::vector<Obstacle> obstacles;
    float scrollSpeed;
    float spawnTimer;
    float spawnInterval;
    float difficulty;
    float distanceTraveled;

    ObstacleManager();

    void reset();
    void update(float dt, float screenWidth);
    void render(Renderer &renderer, Shader &shader,
                const Mat4 &proj, float groundY, float time);

    // check collision against player AABB
    bool checkCollision(Vec2 playerPos, float playerSize) const;

private:
    void spawn(float screenWidth, float groundY);
};

#endif
