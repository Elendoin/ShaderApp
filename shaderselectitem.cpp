#include "shaderselectitem.h"
#include "inspectwindow.h"
#include <ui_shaderselectitem.h>
#include <QMouseEvent>
#include <QStyle>
#include <QMenu>
#include <QMainWindow>

ShaderSelectItem::ShaderSelectItem(ShaderModel model, EditMode mode, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::shaderselectitem)
{
    m_isCustom = mode == EditMode::CUSTOM ? true : false;
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
}

void ShaderSelectItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
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

void ShaderSelectItem::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    auto model = m_model;
    QObject::connect(menu.addAction("Inspect"), &QAction::triggered, [this, model]()
    {

        InspectWindow* win = new InspectWindow(model, nullptr);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->setWindowModality(Qt::WindowModal);
        win->show();
    });
    menu.addSeparator();
    menu.addAction("Edit");
    menu.addAction("Remove");



    menu.exec(event->globalPos());
}


ShaderSelectItem::~ShaderSelectItem()
{
    delete ui;
}
