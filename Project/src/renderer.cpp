#include "renderer.h"


// 5x7 bitmap font data (0-9, A-Z, space, :, -, ., !, >)

const unsigned char Renderer::fontData[FONT_CHARS][FONT_H] = {
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, // 0
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}, // 1
    {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}, // 2
    {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E}, // 3
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, // 4
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}, // 5
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}, // 6
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}, // 7
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, // 8
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}, // 9
    {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}, // A
    {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, // B
    {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, // C
    {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}, // D
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}, // E
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}, // F
    {0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}, // G
    {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}, // H
    {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}, // I
    {0x07,0x02,0x02,0x02,0x02,0x12,0x0C}, // J
    {0x11,0x12,0x14,0x18,0x14,0x12,0x11}, // K
    {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}, // L
    {0x11,0x1B,0x15,0x15,0x11,0x11,0x11}, // M
    {0x11,0x19,0x15,0x13,0x11,0x11,0x11}, // N
    {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}, // O
    {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}, // P
    {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}, // Q
    {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}, // R
    {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E}, // S
    {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}, // T
    {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}, // U
    {0x11,0x11,0x11,0x11,0x0A,0x0A,0x04}, // V
    {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}, // W
    {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}, // X
    {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}, // Y
    {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}, // Z
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // space
    {0x00,0x04,0x04,0x00,0x04,0x04,0x00}, // :
    {0x00,0x00,0x00,0x1F,0x00,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x00,0x04,0x04}, // .
    {0x04,0x04,0x04,0x04,0x04,0x00,0x04}, // !
    {0x10,0x08,0x04,0x02,0x04,0x08,0x10}, // >
};

int Renderer::charIndex(char c) const
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 10 + (c - 'a');
    if (c == ' ')  return 36;
    if (c == ':')  return 37;
    if (c == '-')  return 38;
    if (c == '.')  return 39;
    if (c == '!')  return 40;
    if (c == '>')  return 41;
    return 36;
}


// Constructor / Destructor

Renderer::Renderer()
    : quadVAO(0), quadVBO(0), quadEBO(0),
      triVAO(0), triVBO(0),
      spikeVAO(0), spikeVBO(0),
      bgVAO(0), bgVBO(0)
{
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadEBO);
    glDeleteVertexArrays(1, &triVAO);
    glDeleteBuffers(1, &triVBO);
    glDeleteVertexArrays(1, &spikeVAO);
    glDeleteBuffers(1, &spikeVBO);
    glDeleteVertexArrays(1, &bgVAO);
    glDeleteBuffers(1, &bgVBO);
}


// Initialize all GPU data

void Renderer::initRenderData()
{
    // -- unit quad (0,0) to (1,1) --
    float qv[] = { 0,0, 1,0, 1,1, 0,1 };
    unsigned int qi[] = { 0,1,2, 2,3,0 };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(qv), qv, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(qi), qi, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // -- right-pointing triangle centered at origin --
    float tv[] = {
         0.577f,  0.0f,    // right tip
        -0.289f,  0.5f,    // top-left
        -0.289f, -0.5f     // bottom-left
    };
    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);
    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tv), tv, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // -- upward-pointing spike triangle --
    float sv[] = {
         0.0f,  1.0f,    // top
        -0.5f,  0.0f,    // bottom-left
         0.5f,  0.0f     // bottom-right
    };
    glGenVertexArrays(1, &spikeVAO);
    glGenBuffers(1, &spikeVBO);
    glBindVertexArray(spikeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, spikeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sv), sv, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glBindVertexArray(0);

    // -- fullscreen quad --
    float bv[] = { -1,-1, 1,-1, 1,1, -1,-1, 1,1, -1,1 };
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);
    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bv), bv, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}


// Draw quad

void Renderer::drawQuad(float x, float y, float w, float h,
                        float r, float g, float b, float a,
                        Shader &shader, const Mat4 &proj)
{
    shader.use();
    Mat4 model = Mat4::identity();
    model = Mat4::translate(model, Vec3(x, y, 0.0f));
    model = Mat4::scale(model, Vec3(w, h, 1.0f));
    shader.setMat4("model", model.data());
    shader.setMat4("projection", proj.data());
    shader.setVec4("color", r, g, b, a);
    shader.setFloat("glowStrength", 0.0f);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}




// Draw right-pointing triangle (player)

void Renderer::drawRightTriangle(float x, float y, float size,
                                 float scaleX, float scaleY, float rotation,
                                 float r, float g, float b, float a,
                                 Shader &shader, const Mat4 &proj)
{
    shader.use();
    Mat4 model = Mat4::identity();
    model = Mat4::translate(model, Vec3(x, y, 0.0f));
    model = Mat4::rotate(model, rotation, Vec3(0.0f, 0.0f, 1.0f));
    model = Mat4::scale(model, Vec3(size * scaleX, size * scaleY, 1.0f));
    shader.setMat4("model", model.data());
    shader.setMat4("projection", proj.data());
    shader.setVec4("color", r, g, b, a);
    shader.setFloat("glowStrength", 0.0f);
    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}




// Draw spike triangle pointing UP

void Renderer::drawSpikeTriangle(float x, float y, float w, float h,
                                 float r, float g, float b, float a,
                                 Shader &shader, const Mat4 &proj)
{
    shader.use();
    Mat4 model = Mat4::identity();
    model = Mat4::translate(model, Vec3(x, y, 0.0f));
    model = Mat4::scale(model, Vec3(w, h, 1.0f));
    shader.setMat4("model", model.data());
    shader.setMat4("projection", proj.data());
    shader.setVec4("color", r, g, b, a);
    shader.setFloat("glowStrength", 0.0f);
    glBindVertexArray(spikeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}




// Draw fullscreen background

void Renderer::drawBackground(Shader &shader)
{
    shader.use();
    glBindVertexArray(bgVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


// Draw bitmap font text

void Renderer::drawText(const char* text, float x, float y, float scale,
                        float r, float g, float b, float a,
                        Shader &shader, const Mat4 &proj)
{
    float px = scale;
    float cursorX = x;
    int len = (int)strlen(text);
    for (int c = 0; c < len; c++)
    {
        int idx = charIndex(text[c]);
        for (int row = 0; row < FONT_H; row++)
        {
            unsigned char bits = fontData[idx][row];
            for (int col = 0; col < FONT_W; col++)
            {
                if (bits & (1 << (FONT_W - 1 - col)))
                {
                    drawQuad(cursorX + col*px, y - row*px,
                             px*0.9f, px*0.9f, r, g, b, a, shader, proj);
                }
            }
        }
        cursorX += (FONT_W + 1) * px;
    }
}
