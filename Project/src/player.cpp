#include "player.h"
#include <cmath>

static const float GRAVITY      = 1800.0f;
static const float JUMP_VEL     = 620.0f;
static const float PLAYER_SIZE  = 26.0f;


// Constructor

Player::Player()
    : size(PLAYER_SIZE), rotation(0.0f), targetRotation(0.0f),
      isAlive(true), onGround(true), jumpCooldown(0.0f)
{
}


// Reset

void Player::reset(float x, float groundY)
{
    pos = Vec2(x, groundY + size * 0.5f);
    vel = Vec2(0.0f, 0.0f);
    rotation = 0.0f;
    targetRotation = 0.0f;
    isAlive = true;
    onGround = true;
    jumpCooldown = 0.0f;
}


// Update physics

void Player::update(float dt, float groundY)
{
    if (!isAlive) return;

    // apply gravity
    vel.y -= GRAVITY * dt;

    // update position
    pos.y += vel.y * dt;

    // ground collision
    float groundContact = groundY + size * 0.5f;
    if (pos.y <= groundContact)
    {
        pos.y = groundContact;
        vel.y = 0.0f;

        if (!onGround)
        {
        }
        onGround = true;
    }
    else
    {
        onGround = false;
    }

    // rotation: spin while in air, snap back on ground
    if (!onGround)
    {
        targetRotation -= 8.0f * dt; // spin clockwise
    }
    else
    {
        // snap rotation to nearest multiple of -PI/2 (quarter turn)
        float snap = roundf(targetRotation / (3.14159f * 0.5f)) * (3.14159f * 0.5f);
        targetRotation = lerpf(targetRotation, snap, 15.0f * dt);
    }
    rotation = lerpf(rotation, targetRotation, 12.0f * dt);



    // cooldown
    if (jumpCooldown > 0.0f) jumpCooldown -= dt;
}


// Jump

void Player::jump()
{
    if (!isAlive) return;
    if (!onGround) return;
    if (jumpCooldown > 0.0f) return;

    vel.y = JUMP_VEL;
    onGround = false;
    jumpCooldown = 0.1f;
}


// Render

void Player::render(Renderer &renderer, Shader &shader,
                    const Mat4 &proj, float time)
{
    if (!isAlive) return;

    // simple red triangle
    renderer.drawRightTriangle(pos.x, pos.y, size,
                               1.0f, 1.0f, rotation,
                               1.0f, 0.2f, 0.2f, 1.0f,
                               shader, proj);
}
