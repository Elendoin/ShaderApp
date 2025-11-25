#include "imagewidget.h"
#include <QOpenGLFramebufferObject>

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
    m_program.setUniformValue("scale", QVector2D(1.0f, 1.0f)); // or compute your custom scaling

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
    if(!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, m_shaderModel.getVertexShaderSource()))
        qDebug() << "Error:" << m_program.log();
    if(!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, m_shaderModel.getFragmentShaderSource()))
        qDebug() << "Error:" << m_program.log();
    if(!m_program.link())
        qDebug() << "Error:" << m_program.log();
}

void ImageWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_image.isNull()) {
        m_texture = std::make_unique<QOpenGLTexture>(m_image);
    }

    if (!m_texture)
        return;

    float widthScale = std::clamp((float)m_image.width()/width(), 0.0f, 1.0f);
    float heightScale = std::clamp((float)m_image.height()/height(), 0.0f, 1.0f);

    updateShader();
    // glEnable(GL_TEXTURE_2D);
    m_texture->bind();
    m_program.bind();

    m_program.setUniformValue("textureSampler", 0);
    m_program.setUniformValue("scale", QVector2D(widthScale, heightScale));

    m_vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_vao.release();

    // glBegin(GL_QUADS);
    // glTexCoord2f(0, 1); glVertex2f(-widthScale, -heightScale);
    // glTexCoord2f(1, 1); glVertex2f(widthScale, -heightScale);
    // glTexCoord2f(1, 0); glVertex2f(widthScale, heightScale);
    // glTexCoord2f(0, 0); glVertex2f(-widthScale, heightScale);
    // glEnd();
    m_texture->release();
}
