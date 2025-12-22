#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec4 color = texture(textureSampler, vTexCoord);

    vec2 centered = vTexCoord - vec2(0.5);
    float dist = length(centered);

    float radius = 0.5;
    float softness = 0.25;

    float vignette = smoothstep(radius, radius - softness, dist);

    color.rgb *= vignette;

    fragColor = color;
}