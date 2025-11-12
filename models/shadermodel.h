#ifndef SHADERMODEL_H
#define SHADERMODEL_H

#include <QString>
#include <QImage>

class ShaderModel
{
public:
    QString name = "Shader";
    QString vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
uniform vec2 scale;
out vec2 vTexCoord;
void main() {
    gl_Position = vec4(position * scale, 0.0, 1.0);
    vTexCoord = texCoord;
}
)";
    QString fragmentShaderSource = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 fragColor;
uniform sampler2D textureSampler;
void main() {
    vec4 color = texture(textureSampler, vTexCoord);
    fragColor = vec4(color.rgb, color.a);
}
)";
    QImage image;
};


#endif // SHADERMODEL_H
