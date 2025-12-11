#include "shaderselectitem.h"
#include "ui_shaderselectitem.h"

shaderselectitem::shaderselectitem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::shaderselectitem)
{
    ui->setupUi(this);
}

shaderselectitem::~shaderselectitem()
{
    delete ui;
}
