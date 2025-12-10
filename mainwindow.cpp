#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/filehelper.h"
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include <shaderlistwindow.h>
#include <QOpenGLShader>
#include <QImageWriter>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(width(), height());

    ImageWidget* imageWidget = new ImageWidget(this);

    ui->imageLayout->addWidget(imageWidget, 1);
    ui->actionSaveFile->setDisabled(true);

    loadRecents(imageWidget);

    QObject::connect(ui->actionOpenFile, &QAction::triggered, [this, imageWidget]()
    {
        auto filePath = QFileDialog::getOpenFileName(this,tr("Open image file"), "", tr("Image (*.jpg *.png, *jpeg);;Image (*jpg);;Image (*png)"));
        loadImageToWidget(fs::path(filePath.toStdString()), imageWidget);
    });

    QObject::connect(ui->actionSaveFile, &QAction::triggered, [this, imageWidget]()
    {
        auto filePath = QFileDialog::getSaveFileName(this,tr("Open image file"), "", tr("JPG (*.jpg);;JPEG (*.jpeg);;PNG (*.png)"));
        QImage saveImage = imageWidget->renderToImage(imageWidget->getImage().width(), imageWidget->getImage().height());

        QImageWriter writer(filePath);
        bool ok = writer.write(saveImage);
        if(!ok)
        {
            qWarning() << "Failed to save image!";
        }
    });

    QObject::connect(ui->actionExit, &QAction::triggered, [this, imageWidget]()
    {
        exit(0);
    });

    QObject::connect(ui->selectFragmentShaderButton, &QPushButton::clicked, [this, imageWidget]()
    {
        ShaderListWindow* win = new ShaderListWindow(QOpenGLShader::Fragment, this);
        win->setWindowTitle("Fragment shader selection");

        QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
        {
            qDebug() << "Selected: " << item->text();
            this->ui->fragmentShaderLabel->setText("Selected Fragment Shader: " + item->text());
            auto map = win->getShaderMap();

            imageWidget->setFragmentShaderSource(map[item->text()]);
            win->accept();
        });

        win->setWindowModality(Qt::WindowModal);
        win->exec();
        win->deleteLater();
    });

    QObject::connect(ui->selectVertexShaderButton, &QPushButton::clicked, [this, imageWidget]()
    {
        ShaderListWindow* win = new ShaderListWindow(QOpenGLShader::Vertex, this);
        win->setWindowTitle("Vertex shader selection");

        QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
        {
            qDebug() << "Selected: " << item->text();
            this->ui->vertexShaderLabel->setText("Selected Vertex Shader: " + item->text());
            auto map = win->getShaderMap();

            imageWidget->setVertexShaderSource(map[item->text()]);
            win->accept();
        });

        win->setWindowModality(Qt::WindowModal);
        win->exec();
        win->deleteLater();
    });
}

void MainWindow::loadRecents(ImageWidget* imageWidget)
{
    auto lines = FileHelper::readLines(fs::current_path() / "recent.txt");
    ui->menuRecent->clear();
    for(const auto& line : lines)
    {
        QAction* action = new QAction(line);
        QObject::connect(action, &QAction::triggered, [this, line, imageWidget]()
        {
            loadImageToWidget(fs::path(line.toStdString()), imageWidget);
        });

        ui->menuRecent->addAction(action);
    }
}

void MainWindow::loadImageToWidget(const fs::path& filePath, ImageWidget* imageWidget)
{
    if (!filePath.empty()) {
        qDebug() << "Selected file:" << filePath;
        QImage image;
        image.load(QString::fromStdString(filePath.generic_string()));

        imageWidget->setImage(image);
        ui->actionSaveFile->setEnabled(true);

        auto lines = FileHelper::readLines(fs::current_path() / "recent.txt");

        if(std::count(lines.begin(), lines.end(), QString::fromStdString(filePath.string())) > 0)
        {
            return;
        }
        else if(lines.size() == 5)
        {
            lines.pop_back();
        }
        lines.insert(lines.begin(), QString::fromStdString(filePath.string()));

        FileHelper::saveLines(lines, fs::current_path() / "recent.txt");
        loadRecents(imageWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
