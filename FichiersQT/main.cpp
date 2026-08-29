#include "mainwindow.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //w.setWindowIcon(QIcon(":/images/---.png")); // pour plus tard

    w.show();
    return QApplication::exec();
}
