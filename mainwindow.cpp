#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include "imagewidget.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
