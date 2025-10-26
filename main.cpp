#include "dedefines.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APPNAME);
    a.setApplicationVersion(APPVER);
    a.setOrganizationName(ORGDOM);
    a.setOrganizationDomain(ORGNAME);
    a.setStyle("fusion");
    MainWindow w;
    w.show();
    return a.exec();
}
