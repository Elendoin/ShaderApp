#ifndef SHADERSELECTITEM_H
#define SHADERSELECTITEM_H

#include <QWidget>
#include "models/shadermodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class shaderselectitem;
}
QT_END_NAMESPACE

class ShaderSelectItem : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderSelectItem(ShaderModel model, QWidget *parent = nullptr);
    ~ShaderSelectItem();

private:
    Ui::shaderselectitem *ui;
    ShaderModel m_model;
};

#endif // SHADERSELECTITEM_H
