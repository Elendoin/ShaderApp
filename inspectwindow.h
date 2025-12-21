#ifndef INSPECTWINDOW_H
#define INSPECTWINDOW_H

#include <QWidget>
#include "models/shadermodel.h"

namespace Ui {
class InspectWindow;
}

class InspectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InspectWindow(ShaderModel& model, bool editable, QWidget *parent = nullptr);
    ~InspectWindow();

private:
    Ui::InspectWindow *ui;
    ShaderModel m_model;
};

#endif // INSPECTWINDOW_H
