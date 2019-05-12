#ifndef DATABASEPARSER_H
#define DATABASEPARSER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QMap>

class dataBaseParser : public QObject
{
    Q_OBJECT
public:
    dataBaseParser(QString dbDriver = "QMYSQL", QString dbHostName = "127.0.0.1", int dbPort = 3306,
                            QString dbUserName = "root", QString password = "root", QObject *parent = nullptr);

    dataBaseParser(QList<QVariant>* data, QObject *parent = nullptr);


    int pushNewRocket(QMap<QString, QVariant> parameters);
    bool deleteRow(int row);
    bool changeRocket(QMap<QString, QVariant> parameters, int row);
    QList<QList<QVariant>> parseCloumns(QStringList columnName, int id = -1);
    bool checkConnect();

private:
    QSqlDatabase db;

    void ChangeDBName(QString name);

};

#endif // DATABASEPARSER_H
