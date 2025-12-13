#ifndef SHADERMODEL_H
#define SHADERMODEL_H

#include <QString>
#include <QImage>

class ShaderModel
{
public:
    QString getName() {return m_name;}
    QString getVertexShaderSource() {return m_vertexShaderSource;}
    QString getFragmentShaderSource() {return m_fragmentShaderSource;}
    QImage getIcon() {return m_icon;}

    void setName(const QString& name) {m_name = name;}
    void setVertexShaderSource(const QString& source) {m_vertexShaderSource = source;}
    void setFragmentShaderSource(const QString& source) {m_fragmentShaderSource = source;}
    void setIcon(const QImage& icon) {m_icon = icon;}

private:
    QString m_name = "BaseShader";
    QString m_vertexShaderSource = R"(
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
    QString m_fragmentShaderSource = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 fragColor;
uniform sampler2D textureSampler;
void main() {
    vec4 color = texture(textureSampler, vTexCoord);
    fragColor = vec4(color.rgb, color.a);
}
)";
    QImage m_icon;
};


#endif // SHADERMODEL_H
