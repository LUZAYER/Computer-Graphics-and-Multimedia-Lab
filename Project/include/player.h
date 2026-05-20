#ifndef PLAYER_H
#define PLAYER_H

#include "math_utils.h"
#include "renderer.h"
#include "shader.h"

class Player
{
public:
    Vec2  pos;
    Vec2  vel;
    float size;
    float rotation;
    float targetRotation;
    bool  isAlive;
    bool  onGround;

    Player();
    void reset(float x, float groundY);
    void update(float dt, float groundY);
    void jump();
    void render(Renderer &renderer, Shader &shader, const Mat4 &proj, float time);

private:
    float jumpCooldown;
};

#endif
