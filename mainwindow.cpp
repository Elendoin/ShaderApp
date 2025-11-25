#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include "imagewidget.h"
#include <shaderlistwindow.h>
#include <QOpenGLShader>
#include <QImageWriter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(width(), height());

    ImageWidget* imageWidget = new ImageWidget(this);

    ui->imageLayout->addWidget(imageWidget, 1);
    ui->actionSaveFile->setDisabled(true);

    QObject::connect(ui->actionOpenFile, &QAction::triggered, [this, imageWidget]()
    {
        auto filePath = QFileDialog::getOpenFileName(this,tr("Open image file"), "", tr("Image (*.jpg *.png, *jpeg);;Image (*jpg);;Image (*png)"));
        if (!filePath.isEmpty()) {
            qDebug() << "Selected file:" << filePath;
            image.load(filePath);

            imageWidget->setImage(image);
            ui->actionSaveFile->setEnabled(true);
        }
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
        win->setWindowFlags(Qt::Window);
        win->setWindowTitle("Fragment shader selection");
        win->show();

        QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
        {
            qDebug() << "Selected: " << item->text();
            this->ui->fragmentShaderLabel->setText("Selected Fragment Shader: " + item->text());
            auto map = win->getShaderMap();

            imageWidget->setFragmentShaderSource(map[item->text()]);
            win->close();
        });
    });

    QObject::connect(ui->selectVertexShaderButton, &QPushButton::clicked, [this, imageWidget]()
    {
        ShaderListWindow* win = new ShaderListWindow(QOpenGLShader::Vertex, this);
        win->setWindowFlags(Qt::Window);
        win->setWindowTitle("Vertex shader selection");
        win->show();

        QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
        {
            qDebug() << "Selected: " << item->text();
            this->ui->vertexShaderLabel->setText("Selected Vertex Shader: " + item->text());
            auto map = win->getShaderMap();

            imageWidget->setVertexShaderSource(map[item->text()]);
            win->close();
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
