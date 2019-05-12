#include "mainwindow/mainwindow.h"
#include <QApplication>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include <QTranslator>
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);


    QApplication a(argc, argv);

    QTranslator translator;
    if (!translator.load("ru_RU.qm", a.applicationDirPath())){
        QMessageBox msgBox;
        msgBox.setText("Cant load qm file to translator");
        msgBox.exec();
    }

    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    a.exec();
}
