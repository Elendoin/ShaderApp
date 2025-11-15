#include <shaderlistwindow.h>
#include <models/shadermodel.h>
#include <ui_shaderlistwindow.h>
#include <QListWidgetItem>

ShaderListWindow::ShaderListWindow(Mode mode, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ShaderListWindow)
{
    ui->setupUi(this);

    ShaderModel baseShaderModel;
    if(mode == Mode::VERTEX)
        m_shaderMap["BaseVertexShader"] = baseShaderModel.getVertexShaderSource();
    else if(mode == Mode::FRAGMENT)
        m_shaderMap["BaseFragmentShader"] = baseShaderModel.getFragmentShaderSource();


    for(auto shader : m_shaderMap)
    {
        QListWidgetItem* item = new QListWidgetItem(shader.first);
        ui->listWidget->addItem(item);
    }
}

QListWidget* ShaderListWindow::getListWidget() { return ui->listWidget; }
std::map<QString, QString>& ShaderListWindow::getShaderMap() { return m_shaderMap; }
ShaderListWindow::~ShaderListWindow() { delete ui; }
