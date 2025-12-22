#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/filehelper.h"
#include "models/shadermodel.h"
#include "utils/modelserialization.h"
#include <shaderselectitem.h>
#include <QFileDialog>
#include <QDebug>
#include <QLayout>
#include <QOpenGLShader>
#include <QImageWriter>
#include <QGuiApplication>
#include <QMessageBox>
#include <ui_shaderselectitem.h>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(width(), height());

    m_watcher = new QFileSystemWatcher(this);
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
    QObject::connect(ui->actionReload, &QAction::triggered, [this, imageWidget]()
    {
        loadShaderTabs(imageWidget);
    });
    QObject::connect(ui->actionReset_Shaders, &QAction::triggered, [this, imageWidget]()
    {
        resetShaderSelection(imageWidget);
    });
    QObject::connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [this, imageWidget]()
    {
        loadShaderTabs(imageWidget);
    });
    QObject::connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this, imageWidget]()
    {
        loadShaderTabs(imageWidget);
    });

    loadShaderTabs(imageWidget);
}

void MainWindow::resetShaderSelection(ImageWidget* imageWidget)
{
    ShaderModel model;
    imageWidget->setFragmentShaderSource(model.getFragmentShaderSource());
    imageWidget->setVertexShaderSource(model.getVertexShaderSource());
    imageWidget->resetTransform();
}

void MainWindow::handleClipboard(ImageWidget* imageWidget)
{
    QClipboard* clipboard = QGuiApplication::clipboard();

    QObject::connect(ui->actionCopy, &QAction::triggered, [this, clipboard, imageWidget]()
    {
        copyImage(clipboard, imageWidget);
    });

    QObject::connect(ui->actionPaste, &QAction::triggered, [this, clipboard, imageWidget]()
    {
        pasteImage(clipboard, imageWidget);
    });

    QObject::connect(ui->applyButton, &QPushButton::clicked, [this, clipboard, imageWidget]()
    {
        copyImage(clipboard, imageWidget);
        pasteImage(clipboard, imageWidget);
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
        ui->applyButton->setEnabled(true);

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
    defaultIcon.load(QString::fromStdString((fs::current_path()/"defaultIcon.jpg").string()));

    ShaderSelectItem::EditMode mode;
    mode = tabName == "custom" ? ShaderSelectItem::EditMode::CUSTOM : ShaderSelectItem::EditMode::BUILTIN;

    //CLEAR TAB PAGE
    QWidget* tabPage = ui->tabWidget->findChild<QWidget*>(tabName);
    clearShaderTab(tabPage, tabName);

    for(auto entry : fs::directory_iterator(path))
    {
        if(!entry.is_directory())
        {
            continue;
        }

        auto model = ModelSerialization::deserializeShaderModel(entry.path(), defaultIcon);

        ShaderSelectItem* item = new ShaderSelectItem(model, mode, ui->centralwidget->findChild<QWidget*>(tabName));

        m_watcher->addPath(QString::fromStdString(model.getPath().parent_path().string()));
        m_watcher->addPath(QString::fromStdString(model.getSourcePath().string()));
        m_watcher->addPath(QString::fromStdString(model.getDocumentationPath().string()));

        QObject::connect(item, &ShaderSelectItem::clicked, this, [model, imageWidget]()
        {
            imageWidget->setFragmentShaderSource(model.getFragmentShaderSource());
        });

        QObject::connect(item, &ShaderSelectItem::refreshQueued, this, [this, imageWidget]()
        {
            loadShaderTabs(imageWidget);
        });

        QWidget* tabPage = ui->tabWidget->findChild<QWidget*>(tabName);
        if (tabPage) {
            QWidget* widget = tabPage->findChild<QWidget*>(tabName + "ScrollAreaWidgetContents");
            if (widget) {
                widget->layout()->addWidget(item);
                widget->layout()->setAlignment(Qt::AlignLeft);
            }
        }
    }

    //ADD MODEL BUTTON
    if(mode == ShaderSelectItem::EditMode::CUSTOM)
    {
        QWidget* contentWidget;
        if (tabPage)
        {
            contentWidget = tabPage->findChild<QWidget*>(tabName + "ScrollAreaWidgetContents");
        }

        ShaderModel addModel;
        QImage modelImage;
        modelImage.load(QString::fromStdString((fs::current_path()/"plusIcon.png").string()));
        addModel.setName("");
        addModel.setIcon(modelImage);

        QWidget* parentWidget = ui->centralwidget->findChild<QWidget*>(tabName);
        ShaderSelectItem* addNewModelItem = new ShaderSelectItem(addModel, mode, parentWidget);
        addNewModelItem->getUi()->iconLabel->setFrameStyle(0);
        addNewModelItem->getUi()->shaderNameLabel->hide();

        QObject::connect(addNewModelItem, &ShaderSelectItem::clicked, this, [this, imageWidget, tabName, defaultIcon]()
        {
            ShaderModel newModel;
            newModel.setIcon(defaultIcon);
            QString name = "NewModel";

            name = FileHelper::findIncrementFileName(name, fs::current_path() / "shaderTabs" / tabName.toStdString());

            auto newPath = fs::current_path() / "shaderTabs" / tabName.toStdString() / name.toStdString();
            fs::create_directory(newPath);
            newModel.setPath(newPath);
            ModelSerialization::serializeShaderModel(newModel);
            loadShaderTabs(imageWidget);
        });

        if (contentWidget)
        {
            contentWidget->layout()->addWidget(addNewModelItem);
            contentWidget->layout()->setAlignment(Qt::AlignLeft);
        }
    }
}

void MainWindow::loadShaderTabs(ImageWidget* imageWidget)
{
    m_watcher->removePaths(m_watcher->files());
    m_watcher->removePaths(m_watcher->directories());
    for(int i = 0; i < ui->tabWidget->count(); i++)
    {
        auto page = ui->tabWidget->widget(i);
        auto title = page->objectName();
        loadShaderTab(title, imageWidget);
    }
}

void MainWindow::clearShaderTab(QWidget* tabPage, QString tabName)
{
    if(tabPage)
    {
        QWidget* contents = tabPage->findChild<QWidget*>(tabName + "ScrollAreaWidgetContents");
        if (contents)
        {
            const auto children = contents->findChildren<QWidget*>(
                QString(), Qt::FindDirectChildrenOnly);

            for (QWidget* child : children)
            {
                child->deleteLater();
            }

            if (QLayout* layout = contents->layout())
            {
                while (QLayoutItem* item = layout->takeAt(0))
                {
                    delete item;
                }
            }
        }
    }
}

void MainWindow::copyImage(QClipboard* clipboard, ImageWidget* imageWidget)
{
    auto size = imageWidget->getImage().size();
    QImage image = imageWidget->renderToImage(size.width(), size.height());
    clipboard->setImage(image);
}

void MainWindow::pasteImage(QClipboard* clipboard, ImageWidget* imageWidget)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
