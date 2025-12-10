#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>
#include "imagewidget.h"
namespace fs = std::filesystem;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void loadRecents(ImageWidget* imageWidget);
    void loadImageToWidget(const fs::path& filePath, ImageWidget* imageWidget);
};
#endif // MAINWINDOW_H
