#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(textureSampler, 0));

    // Sobel kernels
    float kernelX[9] = float[](
        -1,  0,  1,
        -2,  0,  2,
        -1,  0,  1
    );

    float kernelY[9] = float[](
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    );

    vec2 offsets[9] = vec2[](
        vec2(-1,  1), vec2(0,  1), vec2(1,  1),
        vec2(-1,  0), vec2(0,  0), vec2(1,  0),
        vec2(-1, -1), vec2(0, -1), vec2(1, -1)
    );

    float gx = 0.0;
    float gy = 0.0;

    for (int i = 0; i < 9; i++)
    {
        vec3 sampleColor = texture(textureSampler, vTexCoord + offsets[i] * texelSize).rgb;
        float intensity = dot(sampleColor, vec3(0.299, 0.587, 0.114)); // grayscale

        gx += intensity * kernelX[i];
        gy += intensity * kernelY[i];
    }

    float edgeStrength = length(vec2(gx, gy));

    fragColor = vec4(vec3(edgeStrength), 1.0);
}