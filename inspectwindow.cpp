#include "inspectwindow.h"
#include "ui_inspectwindow.h"
#include <QMessageBox>
#include <utils/modelserialization.h>
#include <utils/filehelper.h>

InspectWindow::InspectWindow(ShaderModel& model, bool editable, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InspectWindow)
{
    m_model = model;
    m_watcher = new QFileSystemWatcher(this);
    loadWatcher();
    ui->setupUi(this);

    loadContent();

    if(!editable)
    {
        ui->editDocumentationButton->hide();
        ui->editSourceButton->hide();
        ui->editNameButton->hide();
    }

    QObject::connect(ui->editNameButton, &QToolButton::clicked, [this]()
    {
        ui->nameEdit->setText(ui->nameLabel->text());

        ui->nameLabel->hide();
        ui->nameEdit->show();

        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
    });

    QObject::connect(ui->nameEdit, &QLineEdit::editingFinished, this, [this]() {
        if(ui->nameEdit->text() == "" || ui->nameEdit->text() == ui->nameLabel->text())
        {
            ui->nameEdit->hide();
            ui->nameLabel->show();
            return;
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Edit Name", "Are you sure you want to change the name to: \"" + ui->nameEdit->text() + "\"?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            auto oldPath = m_model.getPath();
            auto newPath = m_model.getPath().parent_path() / ui->nameEdit->text().toStdString();

            std::filesystem::copy(m_model.getPath(), newPath);
            m_model = ModelSerialization::deserializeShaderModel(newPath, m_model.getIcon());
            fs::remove_all(oldPath);

            loadContent();
            loadWatcher();
            emit refreshQueued();
        }
        else
        {
            ui->nameEdit->hide();
            ui->nameLabel->show();
            return;
        }

        ui->nameLabel->setText(ui->nameEdit->text());
        ui->nameEdit->hide();
        ui->nameLabel->show();
    });

    QObject::connect(ui->editSourceButton, &QToolButton::clicked, [this]()
    {
        FileHelper::windowsOpenFileAs(m_model.getSourcePath());
    });

    QObject::connect(ui->editDocumentationButton, &QToolButton::clicked, [this]()
    {
        if(!fs::exists(m_model.getDocumentationPath()))
        {
            auto docPath = m_model.getPath() / "documentation.md";
            FileHelper::saveString("", docPath);
            m_model.setDocumentationPath(docPath);
        }
        FileHelper::windowsOpenFileAs(m_model.getDocumentationPath());
    });

    QObject::connect(ui->expandSourceButton, &QToolButton::clicked, [this, editable]()
    {
        ui->horizontalDataLayout->setVisible(!ui->horizontalDataLayout->isVisible());
        ui->documentationLabel->setVisible(!ui->documentationLabel->isVisible());
        ui->documentationWidget->setVisible(!ui->documentationWidget->isVisible());

        if(!editable)
        {
            ui->editDocumentationButton->hide();
            ui->editSourceButton->hide();
            ui->editNameButton->hide();
        }
    });

    QObject::connect(ui->expandDocumentationButton, &QToolButton::clicked, [this, editable]()
    {
        ui->horizontalDataLayout->setVisible(!ui->horizontalDataLayout->isVisible());
        ui->sourceLabel->setVisible(!ui->sourceLabel->isVisible());
        ui->sourceWidget->setVisible(!ui->sourceWidget->isVisible());

        if(!editable)
        {
            ui->editDocumentationButton->hide();
            ui->editSourceButton->hide();
            ui->editNameButton->hide();
        }
    });

    QObject::connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [this]()
    {
        this->close();
    });

    QObject::connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this]()
    {
        auto model = ModelSerialization::deserializeShaderModel(m_model.getPath(), m_model.getIcon());
        m_model = model;
        loadContent();
    });
}

void InspectWindow::loadContent()
{
    ui->nameEdit->setGeometry(ui->nameLabel->geometry());
    ui->nameEdit->hide();

    ui->nameLabel->setText(m_model.getName());
    ui->pathLabel->setText(QString::fromStdString(m_model.getPath().string()));
    ui->sourceTextEdit->setText(m_model.getFragmentShaderSource());
    ui->documentationTextEdit->setMarkdown(m_model.getDocumentation());
}

void InspectWindow::loadWatcher()
{
    m_watcher->addPath(QString::fromStdString(m_model.getPath().parent_path().string()));
    m_watcher->addPath(QString::fromStdString(m_model.getDocumentationPath().string()));
    m_watcher->addPath(QString::fromStdString(m_model.getSourcePath().string()));
}

InspectWindow::~InspectWindow()
{
    delete ui;
}
