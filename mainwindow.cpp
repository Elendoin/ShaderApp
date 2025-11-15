#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include "imagewidget.h"
#include <shaderlistwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(width(), height());

    ImageWidget* imageWidget = new ImageWidget(this);

    ui->imageLayout->addWidget(imageWidget, 1);

    QObject::connect(ui->selectFileButton, &QPushButton::clicked, [this, imageWidget]()
    {
        auto fileName = QFileDialog::getOpenFileName(this,tr("Open image file"), "", tr("Image (*.jpg *.png);;Image (*jpg);;Image (*png)"));
        if (!fileName.isEmpty()) {
            qDebug() << "Selected file:" << fileName;
            image.load(fileName);

            imageWidget->setImage(image);
        }
    });

    QObject::connect(ui->selectFragmentShaderButton, &QPushButton::clicked, [this, imageWidget]()
    {
        ShaderListWindow* win = new ShaderListWindow(ShaderListWindow::FRAGMENT, this);
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
        ShaderListWindow* win = new ShaderListWindow(ShaderListWindow::VERTEX, this);
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
