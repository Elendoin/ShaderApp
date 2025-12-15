#include "shaderselectitem.h"
#include <ui_shaderselectitem.h>
#include <QMouseEvent>
#include <QStyle>

ShaderSelectItem::ShaderSelectItem(ShaderModel model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::shaderselectitem)
{
    m_model = model;
    ui->setupUi(this);

    ui->shaderNameLabel->setText(m_model.getName());
    ui->iconLabel->setPixmap(QPixmap::fromImage(m_model.getIcon()));

    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_Hover, true);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(R"(
ShaderSelectItem {
    background: transparent;
    border-radius: 4px;
}

ShaderSelectItem:hover {
    background: palette(alternate-base);
}

ShaderSelectItem[pressed="true"] {
    background: palette(dark);
}
)");

    qDebug() << metaObject()->className();
    qDebug() << objectName();
}

void ShaderSelectItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setProperty("pressed", true);
        style()->polish(this);
        update();
    }
    QWidget::mousePressEvent(event);
}

void ShaderSelectItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (property("pressed").toBool()) {
        setProperty("pressed", false);
        style()->polish(this);
        update();

        if (rect().contains(event->pos()))
            emit clicked();
    }
    QWidget::mouseReleaseEvent(event);
}


ShaderSelectItem::~ShaderSelectItem()
{
    delete ui;
}
