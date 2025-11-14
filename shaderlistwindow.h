#ifndef SHADERLISTWINDOW_H
#define SHADERLISTWINDOW_H

#include <QWidget>
#include <models/shaderlistmodel.h>
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
    explicit ShaderListWindow(QWidget* parent = nullptr);
    ~ShaderListWindow();
    QListWidget* getListWidget();
    ShaderListModel getListModel();
private:
    Ui::ShaderListWindow *ui;
    ShaderListModel m_shaderListModel;
};

#endif // SHADERLISTWINDOW_H
