#include "mainwindow/mainwindow.h"
#include <QApplication>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.exec();
}
