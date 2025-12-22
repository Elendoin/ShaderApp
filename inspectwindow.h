#ifndef INSPECTWINDOW_H
#define INSPECTWINDOW_H

#include <QWidget>
#include <QFileSystemWatcher>
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

signals:
    void refreshQueued();

private:
    Ui::InspectWindow *ui;
    QFileSystemWatcher* m_watcher;
    ShaderModel m_model;
    void loadContent();
    void loadWatcher();
};

#endif // INSPECTWINDOW_H
