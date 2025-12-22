#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec2 texSize = vec2(textureSize(textureSampler, 0));
    vec2 texel = 1.0 / texSize;

    const int RANGE = 5;

    vec4 bestColor = texture(textureSampler, vTexCoord);
    float bestLuma = dot(bestColor.rgb, vec3(0.299, 0.587, 0.114));

    for (int i = -RANGE; i <= RANGE; i++)
    {
        vec2 offset = vec2(float(i), 0.0) * texel;
        vec4 sampleColor = texture(textureSampler, vTexCoord + offset);

        float luma = dot(sampleColor.rgb, vec3(0.299, 0.587, 0.114));

        if (luma > bestLuma)
        {
            bestLuma = luma;
            bestColor = sampleColor;
        }
    }

    fragColor = bestColor;
}