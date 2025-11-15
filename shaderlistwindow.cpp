#include <filesystem>
#include <shaderlistwindow.h>
#include <models/shadermodel.h>
#include <ui_shaderlistwindow.h>
#include <QListWidgetItem>
#include <utils/filehelper.h>
namespace fs = std::filesystem;

ShaderListWindow::ShaderListWindow(Mode mode, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ShaderListWindow)
{
    ui->setupUi(this);

    ShaderModel baseShaderModel;
    if(mode == Mode::VERTEX)
    {
        m_shaderMap["BaseVertexShader"] = baseShaderModel.getVertexShaderSource();

        for(const auto& entry: fs::directory_iterator(fs::current_path() / "vertexshaders"))
        {
            auto fileName = FileHelper::getFileNameFromPath(entry.path());
            m_shaderMap[fileName] = FileHelper::readFile(entry.path());
        }

    }
    else if(mode == Mode::FRAGMENT)
    {
        m_shaderMap["BaseFragmentShader"] = baseShaderModel.getFragmentShaderSource();

        for(const auto& entry: fs::directory_iterator(fs::current_path() / "fragmentShaders"))
        {
            auto fileName = FileHelper::getFileNameFromPath(entry.path());
            m_shaderMap[fileName] = FileHelper::readFile(entry.path());
        }
    }

    for(auto shader : m_shaderMap)
    {
        QListWidgetItem* item = new QListWidgetItem(shader.first);
        ui->listWidget->addItem(item);
    }
}

QListWidget* ShaderListWindow::getListWidget() { return ui->listWidget; }
std::map<QString, QString>& ShaderListWindow::getShaderMap() { return m_shaderMap; }
ShaderListWindow::~ShaderListWindow() { delete ui; }
