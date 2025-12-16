#include "inspectwindow.h"
#include "ui_inspectwindow.h"

InspectWindow::InspectWindow(ShaderModel model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InspectWindow)
{
    m_model = model;
    ui->setupUi(this);

    ui->nameLabel->setText(m_model.getName());
    ui->pathLabel->setText(QString::fromStdString(m_model.getPath().string()));
    ui->sourceTextEdit->setText(m_model.getFragmentShaderSource());
    ui->documentationTextEdit->setMarkdown(m_model.getDocumentation());

    QObject::connect(ui->expandSourceButton, &QToolButton::clicked, [this]()
    {
        ui->dataWidget->setVisible(!ui->dataWidget->isVisible());
        ui->documentationWidget->setVisible(!ui->documentationWidget->isVisible());
    });

    QObject::connect(ui->expandDocumentationButton, &QToolButton::clicked, [this]()
    {
        ui->dataWidget->setVisible(!ui->dataWidget->isVisible());
        ui->sourceWidget->setVisible(!ui->sourceWidget->isVisible());
    });
}

InspectWindow::~InspectWindow()
{
    delete ui;
}
