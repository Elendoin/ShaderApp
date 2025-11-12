#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <models/shadermodel.h>


class ImageWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = nullptr) {}
    ~ImageWidget() = default;

    void setImage(const QImage& image);
    void setFragmentShader(QString& fragmentShaderSource);
    void setVertexShader(QString& vertexShaderSource);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void updateShader();
    std::unique_ptr<QOpenGLTexture> m_texture;
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QImage m_image;
    ShaderModel m_shaderModel;

};


#endif // IMAGEWIDGET_H


