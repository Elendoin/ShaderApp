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
#include <QPointF>


class ImageWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = nullptr) {}
    ~ImageWidget() = default;

    QImage getImage();
    float getZoom() {return m_scaleMultiplier;}
    QString getShaderName() {return m_shaderModel.getName();}
    void setImage(const QImage &image);
    void setVertexShaderSource(const QString& source);
    void setFragmentShaderSource(const ShaderModel source);
    QImage renderToImage(int width, int height);
    void resetTransform();

signals:
    void zoomChanged();
    void shaderModelChanged();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void updateShader();

    float m_scaleMultiplier = 1.0f;
    QPointF m_lastPos;
    QPointF m_offset;

    std::unique_ptr<QOpenGLTexture> m_texture;
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QImage m_image;
    ShaderModel m_shaderModel;
};


#endif // IMAGEWIDGET_H


