#include "obstacle.h"
#include <cmath>

static const float BASE_SPEED      = 280.0f;
static const float MAX_SPEED       = 650.0f;
static const float BASE_INTERVAL   = 1.8f;
static const float MIN_INTERVAL    = 0.6f;
static const float GROUND_Y        = 100.0f;


// Constructor

ObstacleManager::ObstacleManager()
    : scrollSpeed(BASE_SPEED), spawnTimer(0.0f),
      spawnInterval(BASE_INTERVAL), difficulty(0.0f),
      distanceTraveled(0.0f)
{
}


// Reset

void ObstacleManager::reset()
{
    obstacles.clear();
    scrollSpeed = BASE_SPEED;
    spawnTimer = 0.0f;
    spawnInterval = BASE_INTERVAL;
    difficulty = 0.0f;
    distanceTraveled = 0.0f;
}


// Update - move obstacles left, spawn new ones

void ObstacleManager::update(float dt, float screenWidth)
{
    // increase difficulty over time
    distanceTraveled += scrollSpeed * dt;
    difficulty = clampf(distanceTraveled / 15000.0f, 0.0f, 1.0f);

    scrollSpeed = lerpf(BASE_SPEED, MAX_SPEED, difficulty);
    spawnInterval = lerpf(BASE_INTERVAL, MIN_INTERVAL, difficulty);

    // move all obstacles left
    for (size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles[i].x -= scrollSpeed * dt;
    }

    // remove off-screen obstacles
    for (size_t i = 0; i < obstacles.size(); )
    {
        if (obstacles[i].x + obstacles[i].width < -50.0f)
            obstacles.erase(obstacles.begin() + i);
        else
            i++;
    }

    // spawn new obstacles
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        spawn(screenWidth, GROUND_Y);
    }
}


// Spawn a new obstacle

void ObstacleManager::spawn(float screenWidth, float groundY)
{
    Obstacle obs;
    obs.x = screenWidth + 20.0f;
    obs.active = true;
    obs.spikeW = 0;
    obs.spikeH = 0;

    float roll = randomFloat(0.0f, 1.0f);

    if (roll < 0.35f)
    {
        // tall block
        obs.type = OBS_BLOCK;
        obs.width = randomFloat(30.0f, 50.0f);
        obs.height = randomFloat(40.0f, 70.0f + difficulty * 30.0f);
        obs.y = groundY;
    }
    else if (roll < 0.65f)
    {
        // ground spike
        obs.type = OBS_SPIKE;
        obs.width = randomFloat(22.0f, 35.0f);
        obs.height = randomFloat(25.0f, 40.0f);
        obs.y = groundY;
        obs.spikeW = obs.width;
        obs.spikeH = obs.height;
    }
    else if (roll < 0.80f && difficulty > 0.2f)
    {
        // double block (two blocks close together)
        obs.type = OBS_DOUBLE_BLOCK;
        obs.width = randomFloat(25.0f, 40.0f);
        obs.height = randomFloat(45.0f, 65.0f);
        obs.y = groundY;

        // spawn second block slightly behind
        Obstacle obs2;
        obs2.type = OBS_BLOCK;
        obs2.x = obs.x + obs.width + randomFloat(60.0f, 100.0f);
        obs2.y = groundY;
        obs2.width = randomFloat(25.0f, 40.0f);
        obs2.height = randomFloat(45.0f, 70.0f);
        obs2.active = true;
        obs2.spikeW = 0;
        obs2.spikeH = 0;
        obstacles.push_back(obs2);
    }
    else
    {
        // low block
        obs.type = OBS_LOW_BLOCK;
        obs.width = randomFloat(40.0f, 70.0f);
        obs.height = randomFloat(25.0f, 35.0f);
        obs.y = groundY;
    }

    obstacles.push_back(obs);
}


// Render all obstacles

void ObstacleManager::render(Renderer &renderer, Shader &shader,
                             const Mat4 &proj, float groundY, float time)
{
    for (size_t i = 0; i < obstacles.size(); i++)
    {
        Obstacle &obs = obstacles[i];
        if (!obs.active) continue;

        if (obs.type == OBS_SPIKE)
        {
            // green spike triangle - solid color, no pulse
            renderer.drawSpikeTriangle(obs.x, obs.y,
                                       obs.spikeW, obs.spikeH,
                                       0.2f, 0.9f, 0.15f, 1.0f,
                                       shader, proj);
        }
        else
        {
            // solid cyan block - no pulse, no grid lines, no interior complexity
            renderer.drawQuad(obs.x, obs.y, obs.width, obs.height,
                              0.0f, 0.85f, 0.9f, 1.0f,
                              shader, proj);
        }
    }
}


// AABB collision check

bool ObstacleManager::checkCollision(Vec2 playerPos, float playerSize) const
{
    float halfSize = playerSize * 0.4f;
    float pLeft   = playerPos.x - halfSize;
    float pRight  = playerPos.x + halfSize;
    float pBottom = playerPos.y - halfSize;
    float pTop    = playerPos.y + halfSize;

    for (size_t i = 0; i < obstacles.size(); i++)
    {
        const Obstacle &obs = obstacles[i];
        if (!obs.active) continue;

        float oLeft   = obs.x;
        float oRight  = obs.x + obs.width;
        float oBottom = obs.y;
        float oTop    = obs.y + obs.height;

        // for spikes, use a tighter hitbox
        if (obs.type == OBS_SPIKE)
        {
            oLeft   = obs.x + obs.spikeW * 0.15f;
            oRight  = obs.x + obs.spikeW * 0.85f;
            oBottom = obs.y;
            oTop    = obs.y + obs.spikeH * 0.8f;
        }

        if (pRight > oLeft && pLeft < oRight &&
            pTop > oBottom && pBottom < oTop)
        {
            return true;
        }
    }

    return false;
}
