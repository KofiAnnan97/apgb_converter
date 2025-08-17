#include "mainwindow.h"

#include<QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
    // Set App Icon
    QIcon appIcon(":/icons/app.ico");
    w.setWindowIcon(appIcon);

    w.show();
    return a.exec();
}
