#include "mainwindow.h"

#include <QApplication>
#include <qsurfaceformat.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Shader App");
    w.show();



    return a.exec();
}
