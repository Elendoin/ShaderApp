#include <filesystem>
#include <qopenglshaderprogram.h>
#include <shaderlistwindow.h>
#include <models/shadermodel.h>
#include <ui_shaderlistwindow.h>
#include <QListWidgetItem>
#include <QFileDialog>
#include <utils/filehelper.h>
#include <QMessageBox>
#include <QProcess>
namespace fs = std::filesystem;

ShaderListWindow::ShaderListWindow(QOpenGLShader::ShaderType shaderType, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ShaderListWindow)
{
    if(shaderType != QOpenGLShader::Fragment && shaderType != QOpenGLShader::Vertex)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid shader type!");
        return;
    }
    ui->setupUi(this);

    modeBody(shaderType);

    reloadView();
}

void ShaderListWindow::modeBody(QOpenGLShader::ShaderType shaderType)
{
    ShaderModel baseShaderModel;
    if(shaderType == QOpenGLShader::Vertex)
    {
        m_shaderMap["BaseVertexShader"] = baseShaderModel.getVertexShaderSource();
    }
    else if (shaderType == QOpenGLShader::Fragment)
    {
        m_shaderMap["BaseFragmentShader"] = baseShaderModel.getFragmentShaderSource();
    }


    auto finalPath = shaderType == QOpenGLShader::Vertex ? "vertexshaders" : "fragmentShaders";
    QObject::connect(ui->importShaderButton, &QPushButton::clicked, [this, finalPath, shaderType]()
    {
        auto filePath = QFileDialog::getOpenFileName(this, tr("Open shader file"), "", tr("Text document (*.txt)"));
        if (!filePath.isEmpty()) {
            qDebug() << "Selected file:" << filePath;
            auto path = fs::path(filePath.toStdString());
            auto contents = FileHelper::readFile(path);

            {
                QOpenGLShaderProgram testProgram;
                if(!testProgram.addShaderFromSourceCode(shaderType, contents))
                {
                    QMessageBox messageBox;
                    messageBox.critical(0,"Import failed","Invalid shader source code!");
                    return;
                }
                testProgram.release();
            }

            FileHelper::saveStringToFile(contents, fs::path(finalPath) / (FileHelper::getFileNameFromPath(path).toStdString() + ".txt"));
            this->m_shaderMap[FileHelper::getFileNameFromPath(path)] = contents;
            this->reloadView();
        }
    });
    QObject::connect(ui->editShaderButton, &QPushButton::clicked, [this, finalPath, shaderType]()
    {
        //TODO: wonky
        QString program = "notepad.exe";
        QStringList arguments;
        arguments << QString::fromStdString((fs::path(finalPath) / (this->getListWidget()->currentItem()->text() + ".txt").toStdString()).generic_string());

        QProcess* process = new QProcess();
        process->startDetached(program, arguments);

        delete(process);
    });


    for(const auto& entry: fs::directory_iterator(fs::current_path() / finalPath))
    {
        auto fileName = FileHelper::getFileNameFromPath(entry.path());
        m_shaderMap[fileName] = FileHelper::readFile(entry.path());
    }
}

void ShaderListWindow::reloadView()
{
    ui->listWidget->clear();
    for(auto shader : m_shaderMap)
    {
        QListWidgetItem* item = new QListWidgetItem(shader.first);
        ui->listWidget->addItem(item);
    }
}

QListWidget* ShaderListWindow::getListWidget() { return ui->listWidget; }
std::map<QString, QString>& ShaderListWindow::getShaderMap() { return m_shaderMap; }
ShaderListWindow::~ShaderListWindow() { delete ui; }
