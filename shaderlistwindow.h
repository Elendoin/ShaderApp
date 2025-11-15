#ifndef SHADERLISTWINDOW_H
#define SHADERLISTWINDOW_H

#include <QWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ShaderListWindow;
}
QT_END_NAMESPACE

class ShaderListWindow : public QWidget
{
    Q_OBJECT
public:
    enum Mode
    {
        FRAGMENT,
        VERTEX
    };
    explicit ShaderListWindow(Mode mode, QWidget* parent = nullptr);
    ~ShaderListWindow();
    std::map<QString, QString>& getShaderMap();
    QListWidget* getListWidget();

private:
    Ui::ShaderListWindow *ui;
    std::map<QString, QString> m_shaderMap;
};

#endif // SHADERLISTWINDOW_H
