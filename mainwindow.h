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


#define MAX_RECENTS 5

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void resetShaderSelection(ImageWidget* imageWidget);
    void handleClipboard(ImageWidget* imageWidget);
    void loadRecents(ImageWidget* imageWidget);
    void loadImageToWidget(const fs::path& filePath, ImageWidget* imageWidget);
    void loadShaderTab(const QString& tabName, ImageWidget* imageWidget);
};
#endif // MAINWINDOW_H
