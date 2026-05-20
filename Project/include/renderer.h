#ifndef RENDERER_H
#define RENDERER_H

#include "glad.h"
#include "shader.h"
#include "math_utils.h"

#include <cstring>

#define FONT_CHARS 42
#define FONT_W 5
#define FONT_H 7

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initRenderData();

    // basic shapes
    void drawQuad(float x, float y, float w, float h,
                  float r, float g, float b, float a,
                  Shader &shader, const Mat4 &proj);



    // triangle pointing RIGHT (player shape)
    void drawRightTriangle(float x, float y, float size,
                           float scaleX, float scaleY, float rotation,
                           float r, float g, float b, float a,
                           Shader &shader, const Mat4 &proj);



    // spike triangle pointing UP (obstacle)
    void drawSpikeTriangle(float x, float y, float w, float h,
                           float r, float g, float b, float a,
                           Shader &shader, const Mat4 &proj);



    // background fullscreen
    void drawBackground(Shader &shader);

    // bitmap text
    void drawText(const char* text, float x, float y, float scale,
                  float r, float g, float b, float a,
                  Shader &shader, const Mat4 &proj);

private:
    unsigned int quadVAO, quadVBO, quadEBO;
    unsigned int triVAO,  triVBO;
    unsigned int spikeVAO, spikeVBO;
    unsigned int bgVAO, bgVBO;

    static const unsigned char fontData[FONT_CHARS][FONT_H];
    int charIndex(char c) const;
};

#endif
