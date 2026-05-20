#version 330 core
out vec4 FragColor;

uniform vec4 color;
uniform float glowStrength;

void main()
{
    float a = color.a * max(1.0 - glowStrength * 0.4, 0.05);
    FragColor = vec4(color.rgb * (1.0 + glowStrength * 0.3), a);
}
