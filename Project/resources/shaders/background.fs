#version 330 core
out vec4 FragColor;

in vec2 fragCoord;

uniform float time;
uniform float scrollX;
uniform vec2  resolution;

void main()
{
    vec2 uv = fragCoord;

    // deep purple-blue gradient
    vec3 topColor    = vec3(0.08, 0.02, 0.25);
    vec3 bottomColor = vec3(0.02, 0.04, 0.35);
    vec3 bg = mix(bottomColor, topColor, uv.y);

    // grid pattern (static)
    vec2 gridUV = vec2(uv.x, uv.y);
    float gx = smoothstep(0.96, 1.0, abs(sin(gridUV.x * 12.0)));
    float gy = smoothstep(0.96, 1.0, abs(sin(gridUV.y * 8.0)));
    float grid = max(gx, gy) * 0.12;
    vec3 gridColor = vec3(0.2, 0.1, 0.5) * grid;

    // secondary grid (static)
    vec2 grid2UV = vec2(uv.x, uv.y);
    float gx2 = smoothstep(0.97, 1.0, abs(sin(grid2UV.x * 25.0)));
    float gy2 = smoothstep(0.97, 1.0, abs(sin(grid2UV.y * 16.0)));
    float grid2 = max(gx2, gy2) * 0.06;
    vec3 grid2Color = vec3(0.15, 0.08, 0.4) * grid2;

    vec3 finalColor = bg + gridColor + grid2Color;
    FragColor = vec4(finalColor, 1.0);
}
