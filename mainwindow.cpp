#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/filehelper.h"
#include "models/shadermodel.h"
#include "utils/modeldeserializer.h"
#include <shaderselectitem.h>
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include <shaderlistwindow.h>
#include <QOpenGLShader>
#include <QImageWriter>
#include <QClipboard>
#include <QGuiApplication>
#include <QMessageBox>

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
    handleClipboard(imageWidget);

    QObject::connect(ui->actionOpenFile, &QAction::triggered, [this, imageWidget]()
    {
        auto filePath = QFileDialog::getOpenFileName(this,tr("Open image file"), "", tr("Image (*.jpg *.png *jpeg);;Image (*jpg);;Image (*png)"));
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

    auto tabWidget = ui->tabWidget;
    auto infoWidget = ui->imageInfo;
    QObject::connect(ui->actionFilters, &QAction::triggered, [this, tabWidget]()
    {
        tabWidget->setVisible(!tabWidget->isVisible());
    });
    QObject::connect(ui->actionInfo, &QAction::triggered, [this, infoWidget]()
    {
        infoWidget->setVisible(!infoWidget->isVisible());
    });

    for(int i = 0; i < tabWidget->count(); i++)
    {
        auto page = tabWidget->widget(i);
        auto title = page->objectName();
        loadShaderTab(title, imageWidget);
    }

    // QObject::connect(ui->selectFragmentShaderButton, &QPushButton::clicked, [this, imageWidget]()
    // {
    //     ShaderListWindow* win = new ShaderListWindow(QOpenGLShader::Fragment, this);
    //     win->setWindowTitle("Fragment shader selection");

    //     QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
    //     {
    //         qDebug() << "Selected: " << item->text();
    //         this->ui->fragmentShaderLabel->setText("Selected Fragment Shader: " + item->text());
    //         auto map = win->getShaderMap();

    //         imageWidget->setFragmentShaderSource(map[item->text()]);
    //         win->accept();
    //     });

    //     win->setWindowModality(Qt::WindowModal);
    //     win->exec();
    //     win->deleteLater();
    // });

    // QObject::connect(ui->selectVertexShaderButton, &QPushButton::clicked, [this, imageWidget]()
    // {
    //     ShaderListWindow* win = new ShaderListWindow(QOpenGLShader::Vertex, this);
    //     win->setWindowTitle("Vertex shader selection");

    //     QObject::connect(win->getListWidget(), &QListWidget::itemDoubleClicked, [this, win, imageWidget](QListWidgetItem* item)
    //     {
    //         qDebug() << "Selected: " << item->text();
    //         this->ui->vertexShaderLabel->setText("Selected Vertex Shader: " + item->text());
    //         auto map = win->getShaderMap();

    //         imageWidget->setVertexShaderSource(map[item->text()]);
    //         win->accept();
    //     });

    //     win->setWindowModality(Qt::WindowModal);
    //     win->exec();
    //     win->deleteLater();
    // });
}

void MainWindow::resetShaderSelection(ImageWidget* imageWidget)
{
    ShaderModel model;
    imageWidget->setFragmentShaderSource(model.getFragmentShaderSource());
    imageWidget->setVertexShaderSource(model.getVertexShaderSource());
}

void MainWindow::handleClipboard(ImageWidget* imageWidget)
{
    QClipboard* clipboard = QGuiApplication::clipboard();

    QObject::connect(ui->actionCopy, &QAction::triggered, [this, clipboard, imageWidget]()
    {
        auto size = imageWidget->getImage().size();
        QImage image = imageWidget->renderToImage(size.width(), size.height());
        clipboard->setImage(image);
    });

    QObject::connect(ui->actionPaste, &QAction::triggered, [this, clipboard, imageWidget]()
    {
        QImage image = clipboard->image();
        if(!image.isNull())
        {
            imageWidget->setImage(image);
            resetShaderSelection(imageWidget);
        }
        else
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Couldn't paste content!");
            return;
        }
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
        ui->actionCopy->setEnabled(true);

        ui->imageSizeLabel->setText("Image size: (" + QString::number(image.width()) + "x" + QString::number(image.height()) +  ")");
        ui->filenameLabel->setText("File name: " + FileHelper::getFileNameFromPath(filePath));

        auto lines = FileHelper::readLines(fs::current_path() / "recent.txt");
        auto size = imageWidget->getImage().size();

        if(std::count(lines.begin(), lines.end(), QString::fromStdString(filePath.string())) > 0)
        {
            return;
        }
        else if(lines.size() == MAX_RECENTS)
        {
            lines.pop_back();
        }
        lines.insert(lines.begin(), QString::fromStdString(filePath.string()));

        FileHelper::saveLines(lines, fs::current_path() / "recent.txt");
        loadRecents(imageWidget);
    }
}

void MainWindow::loadShaderTab(const QString& tabName, ImageWidget* imageWidget)
{
    auto path = fs::current_path() / "shaderTabs" / tabName.toStdString();
    QImage defaultIcon;
    defaultIcon.load(QString::fromStdString((fs::current_path()/"defaultIcon.png").string()));

    for(auto entry : fs::directory_iterator(path))
    {
        if(!entry.is_directory())
        {
            continue;
        }

        auto model = ModelDeserializer::deserializeShaderModel(entry.path(), defaultIcon);

        ShaderSelectItem::EditMode mode;
        mode = tabName == "Custom" ? ShaderSelectItem::EditMode::CUSTOM : ShaderSelectItem::EditMode::BUILTIN;

        ShaderSelectItem* item = new ShaderSelectItem(model, mode, ui->centralwidget->findChild<QWidget*>(tabName));
        QObject::connect(item, &ShaderSelectItem::clicked, this, [model, imageWidget]()
        {
             imageWidget->setFragmentShaderSource(model.getFragmentShaderSource());
        });

        QWidget* tabPage = ui->tabWidget->findChild<QWidget*>(tabName);
        if (tabPage) {
            QWidget* widget = tabPage->findChild<QWidget*>(tabName + "ScrollAreaWidgetContents");
            if (widget) {
                widget->layout()->addWidget(item);
                widget->layout()->setAlignment(Qt::AlignLeft);
            }
        }
        //ui->colorTab->layout()->setAlignment(Qt::AlignLeft);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
