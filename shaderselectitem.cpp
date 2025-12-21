#include "shaderselectitem.h"
#include "inspectwindow.h"
#include "utils/filehelper.h"
#include <ui_shaderselectitem.h>
#include <QMouseEvent>
#include <QStyle>
#include <QMenu>
#include <QMainWindow>
#include <QMessageBox>

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

    if(!m_isCustom)
    {
        QAction* inspectAction = new QAction("Inspect");
        menu.addAction(inspectAction);
        QObject::connect(inspectAction, &QAction::triggered, [this]()
        {
            openInspectWindow(false);
        });
    }


    if(m_isCustom && m_model.getName() != "")
    {
        QAction* editAction = new QAction("Inspect / Edit");
        QObject::connect(editAction, &QAction::triggered, [this]()
        {
            openInspectWindow(true);
        });


        QAction* removeAction = new QAction("Remove");
        QObject::connect(removeAction, &QAction::triggered, [this]()
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete: \"" + m_model.getName() + "\"?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                FileHelper::deleteDirectory(m_model.getPath());
                emit refreshQueued();
            }
        });

        menu.addAction(editAction);
        menu.addSeparator();
        menu.addAction(removeAction);
    }

    menu.exec(event->globalPos());
}

void ShaderSelectItem::openInspectWindow(bool editable)
{
    InspectWindow* win = new InspectWindow(m_model, editable, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->setWindowModality(Qt::WindowModal);
    win->show();
}

ShaderSelectItem::~ShaderSelectItem()
{
    delete ui;
}
