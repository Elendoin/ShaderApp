#ifndef SHADERLISTWINDOW_H
#define SHADERLISTWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QOpenGLShader>

QT_BEGIN_NAMESPACE
namespace Ui {
class ShaderListWindow;
}
QT_END_NAMESPACE

class ShaderListWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ShaderListWindow(QOpenGLShader::ShaderType shaderType, QWidget* parent = nullptr);
    ~ShaderListWindow();
    std::map<QString, QString>& getShaderMap();
    QListWidget* getListWidget();

private:
    void modeBody(QOpenGLShader::ShaderType shaderType);
    void reloadView();
    Ui::ShaderListWindow *ui;
    std::map<QString, QString> m_shaderMap;
};

#endif // SHADERLISTWINDOW_H
