#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec2 centered = vTexCoord - vec2(0.5);

    float r = length(centered);

    float strength = 1.5;

    // Apply fisheye distortion
    float theta = atan(centered.y, centered.x);
    float distortedR = pow(r, strength);

    vec2 distortedCoord = distortedR * vec2(cos(theta), sin(theta));

    vec2 uv = distortedCoord + vec2(0.5);

    vec4 color = texture(textureSampler, uv);

    fragColor = color;
}