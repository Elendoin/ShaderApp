#include "shaderselectitem.h"
#include <ui_shaderselectitem.h>

ShaderSelectItem::ShaderSelectItem(ShaderModel model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::shaderselectitem)
{
    m_model = model;
    ui->setupUi(this);

    ui->shaderNameLabel->setText(m_model.getName());
    ui->iconLabel->setPixmap(QPixmap::fromImage(m_model.getIcon()));
}

ShaderSelectItem::~ShaderSelectItem()
{
    delete ui;
}
