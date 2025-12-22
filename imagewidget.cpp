#include "imagewidget.h"
#include <QOpenGLFramebufferObject>
#include <QMessageBox>
#include <QWheelEvent>

QImage ImageWidget::getImage()
{
    return m_image;
}

void ImageWidget::setImage(const QImage &image)
{
    m_image = image;
    update();
    qDebug() << "Widget: (" << width() << ", " << height() << ")";
    qDebug() << "Image: (" << image.width() << ", " << image.height() << ")";
    qDebug() << "Scale: (" << (float)image.width()/width() << ", " << (float)image.height()/height() << ")";
    qDebug() << "Vertex Shader Code: " << m_shaderModel.getVertexShaderSource();
    qDebug() << "Fragment Shader Code: " << m_shaderModel.getFragmentShaderSource();
    qDebug() << Qt::endl;
}

void ImageWidget::setVertexShaderSource(const QString& source)
{
    m_shaderModel.setVertexShaderSource(source);
    //TODO - change so it checks for context, also don't render shaders everytime
    setImage(m_image);
}

void ImageWidget::setFragmentShaderSource(const QString& source)
{
    m_shaderModel.setFragmentShaderSource(source);
    //TODO - change so it checks for context, also don't render shaders everytime
    setImage(m_image);
}

QImage ImageWidget::renderToImage(int width, int height)
{
    makeCurrent();

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setTextureTarget(GL_TEXTURE_2D);
    QOpenGLFramebufferObject fbo(width, height, format);

    fbo.bind();

    glViewport(0, 0, width, height);

    updateShader();

    m_texture->bind();
    m_program.bind();

    m_program.setUniformValue("textureSampler", 0);
    m_program.setUniformValue("scale", QVector2D(1.0f, 1.0f));

    m_vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_vao.release();

    m_texture->release();
    m_program.release();

    QImage result = fbo.toImage();
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    doneCurrent();
    return result;
}

void ImageWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    float vertices[] = {
    //Position    //Texture
    -1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 0.0f
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};


    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    m_vao.release();
    m_vbo.release();

    updateShader();
}

void ImageWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w, h);
}

void ImageWidget::updateShader()
{
    m_program.removeAllShaders();
    if (!m_program.addShaderFromSourceCode(
            QOpenGLShader::Vertex,
            m_shaderModel.getVertexShaderSource()))
    {
        QMessageBox::critical(this, "Vertex Shader Error", m_program.log());
        return;
    }

    if (!m_program.addShaderFromSourceCode(
            QOpenGLShader::Fragment,
            m_shaderModel.getFragmentShaderSource()))
    {
        QMessageBox::critical(this, "Fragment Shader Error", m_program.log());
        return;
    }

    if (!m_program.link())
    {
        QMessageBox::critical(this, "Shader Link Error", m_program.log());
        return;
    }
}

void ImageWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_image.isNull()) {
        m_texture = std::make_unique<QOpenGLTexture>(m_image);
    }

    if (!m_texture)
        return;

    float widgetAspect = (float)width() / height();
    float imageAspect  = (float)m_image.width() / m_image.height();

    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (imageAspect > widgetAspect) {
        scaleX = 1.0f;
        scaleY = widgetAspect / imageAspect;
    } else {
        scaleX = imageAspect / widgetAspect;
        scaleY = 1.0f;
    }

    updateShader();
    // glEnable(GL_TEXTURE_2D);
    m_texture->bind();
    m_program.bind();

    m_program.setUniformValue("textureSampler", 0);
    m_program.setUniformValue("scale", QVector2D(scaleX * m_scaleMultiplier, scaleY * m_scaleMultiplier));
    m_program.setUniformValue("offset", QVector2D(m_offset));

    m_vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_vao.release();

    m_texture->release();
}

void ImageWidget::resetTransform()
{
    m_scaleMultiplier = 1.0f;
    m_offset = QPointF();
    update();
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_lastPos = event->pos();
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        auto delta = event->position() - m_lastPos;
        m_lastPos = event->position();

        m_offset += QPointF(delta.x(), -delta.y()) * 0.005f;
        update();
    }
}

void ImageWidget::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() > 0)
    {
        m_scaleMultiplier += 0.05f;
    }
    else if(event->angleDelta().y() < 0)
    {
        m_scaleMultiplier -= 0.05f;
    }
    update();

    event->accept();
}
