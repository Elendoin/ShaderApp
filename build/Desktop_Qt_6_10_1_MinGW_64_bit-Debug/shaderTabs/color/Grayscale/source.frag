#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec4 color = texture(textureSampler, vTexCoord);

    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    fragColor = vec4(vec3(gray), color.a);
}