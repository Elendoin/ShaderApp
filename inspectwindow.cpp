#include "inspectwindow.h"
#include "ui_inspectwindow.h"

InspectWindow::InspectWindow(ShaderModel& model, bool editable, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InspectWindow)
{
    m_model = model;
    ui->setupUi(this);

    ui->nameLabel->setText(m_model.getName());
    ui->pathLabel->setText(QString::fromStdString(m_model.getPath().string()));
    ui->sourceTextEdit->setText(m_model.getFragmentShaderSource());
    ui->documentationTextEdit->setMarkdown(m_model.getDocumentation());

    if(editable)
    {

    }
    else
    {
        ui->editDocumentationButton->hide();
        ui->editSourceButton->hide();
        ui->editNameButton->hide();
    }

    QObject::connect(ui->expandSourceButton, &QToolButton::clicked, [this]()
    {
        ui->dataWidget->setVisible(!ui->dataWidget->isVisible());
        ui->documentationWidget->setVisible(!ui->documentationWidget->isVisible());
        ui->editDocumentationButton->hide();
        ui->editSourceButton->hide();
    });

    QObject::connect(ui->expandDocumentationButton, &QToolButton::clicked, [this]()
    {
        ui->dataWidget->setVisible(!ui->dataWidget->isVisible());
        ui->sourceWidget->setVisible(!ui->sourceWidget->isVisible());
        ui->editDocumentationButton->hide();
        ui->editSourceButton->hide();
    });
}

InspectWindow::~InspectWindow()
{
    delete ui;
}
