#include <shaderlistwindow.h>
#include <ui_shaderlistwindow.h>
#include <QListWidgetItem>

ShaderListWindow::ShaderListWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ShaderListWindow)
{
    ui->setupUi(this);

    ShaderModel baseShaderModel;
    auto& list = m_shaderListModel.getList();
    list.push_back(baseShaderModel);

    for(auto model : list)
    {
        QListWidgetItem* item = new QListWidgetItem(model.getName());
        ui->listWidget->addItem(item);
    }
}

QListWidget* ShaderListWindow::getListWidget() { return ui->listWidget; }
ShaderListModel ShaderListWindow::getListModel() { return m_shaderListModel; }
ShaderListWindow::~ShaderListWindow() { delete ui; }
