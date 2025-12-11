#ifndef SHADERSELECTITEM_H
#define SHADERSELECTITEM_H

#include <QWidget>

namespace Ui {
class shaderselectitem;
}

class shaderselectitem : public QWidget
{
    Q_OBJECT

public:
    explicit shaderselectitem(QWidget *parent = nullptr);
    ~shaderselectitem();

private:
    Ui::shaderselectitem *ui;
};

#endif // SHADERSELECTITEM_H
