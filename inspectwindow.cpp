#include "inspectwindow.h"
#include "ui_inspectwindow.h"
#include <QMessageBox>

InspectWindow::InspectWindow(ShaderModel& model, bool editable, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InspectWindow)
{
    m_model = model;
    ui->setupUi(this);

    ui->nameEdit->setGeometry(ui->nameLabel->geometry());
    ui->nameEdit->hide();

    ui->nameLabel->setText(m_model.getName());
    ui->pathLabel->setText(QString::fromStdString(m_model.getPath().string()));
    ui->sourceTextEdit->setText(m_model.getFragmentShaderSource());
    ui->documentationTextEdit->setMarkdown(m_model.getDocumentation());

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
}

InspectWindow::~InspectWindow()
{
    delete ui;
}
