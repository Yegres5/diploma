#include "databaseparser.h"
#include <QVariant>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>

dataBaseParser::dataBaseParser(QString dbDriver, QString dbHostName, int dbPort, QString dbUserName, QString password, QObject *parent) : QObject(parent),
    db(QSqlDatabase::addDatabase(dbDriver))
{
    db.setHostName(dbHostName);
    db.setPort(dbPort);
    db.setUserName(dbUserName);
    db.setPassword(password);
    ChangeDBName("programmDB");
}

void dataBaseParser::ChangeDBName(QString name)
{
    db.setDatabaseName(name);
}

QList<QList<QVariant> > dataBaseParser::parseCloumns(QStringList columnName, int id)
{
    QList<QList<QVariant>> list;
    if (db.open()){
          QSqlQuery query(db);
          QString names;
          for (auto& s : columnName){
              names.append("," + s);
          }
          names.remove(0,1);
          if (id == -1){
                query.prepare(QString("SELECT %1 FROM programmDB.Rockets").arg(names));
          }else{
                query.prepare(QString("SELECT %1 FROM programmDB.Rockets WHERE ID = %2").arg(names).arg(id));
          }
          if (query.exec()){
              while (query.next()){
                  QList<QVariant> lst;
                  for (int j(0);j<query.record().count();j++){
                      if (j!=2){
                      lst.push_back(query.value(j));
                      }else{
                          QByteArray outByteArray = query.value(j).toByteArray();
                          QImage im;
                          im.loadFromData(outByteArray);
                          lst.push_back(QVariant(im));
                      }
                  }
                  list.push_back(lst);
              }
          }else{
              QMessageBox msgBox;
              msgBox.setText(query.lastError().text());
              msgBox.exec();
          }
          db.close();
    }
    return list;
}

bool dataBaseParser::deleteRow(int row)
{
    if (db.open()){
        QSqlQuery query(db);
        query.prepare(QString("DELETE FROM `programmDB`.`Rockets` WHERE (`ID` = '%1')").arg(row));
        if  (!query.exec()){
            db.close();
            QMessageBox msgBox;
            msgBox.setText(query.lastError().text());
            msgBox.exec();
            return false;
        }
    }
    return true;
}

bool dataBaseParser::changeRocket(QMap<QString, QVariant> parameters, int row)
{
    if (db.open()){
        QSqlQuery query(db);
        query.prepare("UPDATE `programmDB`.`Rockets` SET `NAME` = :name, `IMAGE` = :image, `Length` = :length, `Diameter` = :diameter, `Ae` = :ae, `Sm` = :sm, `Ln` = :ln,"
                      " `Nymax` = :nymax, `ExplodeDistance` = :explodeDist, `MaxAngle` = :maxAngle WHERE (`ID` = :id);");

        query.bindValue(":name", parameters.find("name")->toByteArray());
        query.bindValue(":image", parameters.find("image")->toByteArray());
        query.bindValue(":length", parameters.find("length")->toString());
        query.bindValue(":diameter", parameters.find("diameter")->toString());
        query.bindValue(":ae", parameters.find("ae")->toString());
        query.bindValue(":sm", parameters.find("sm")->toString());
        query.bindValue(":ln", parameters.find("ln")->toString());
        query.bindValue(":nymax", parameters.find("nymax")->toString());
        query.bindValue(":explodeDist", parameters.find("explodeDist")->toString());
        query.bindValue(":maxAngle", parameters.find("maxAngle")->toString());
        query.bindValue(":id", row);

        if(!query.exec()){
            db.close();
            QMessageBox msgBox;
            msgBox.setText(query.lastError().text());
            msgBox.exec();
            return false;
        }
    }
    return true;
}

bool dataBaseParser::pushNewRocket(QMap<QString, QVariant> parameters)
{
    if (db.open()){
        QSqlQuery query(db);
        query.prepare("INSERT INTO `programmDB`.`Rockets` (`NAME`, `IMAGE`, `Length`, `Diameter`, `Ae`, `Sm`, `Ln`, `Nymax`, `ExplodeDistance`, `MaxAngle`) "
                      "VALUES (:name, :image, :length, :diameter, :ae, :sm, :ln, :nymax, :explodeDist, :maxAngle)");

        query.bindValue(":name", parameters.find("name")->toString());
        query.bindValue(":image", parameters.find("image")->toByteArray());
        query.bindValue(":length", parameters.find("length")->toString());
        query.bindValue(":diameter", parameters.find("diameter")->toString());
        query.bindValue(":ae", parameters.find("ae")->toString());
        query.bindValue(":sm", parameters.find("sm")->toString());
        query.bindValue(":ln", parameters.find("ln")->toString());
        query.bindValue(":nymax", parameters.find("nymax")->toString());
        query.bindValue(":explodeDist", parameters.find("explodeDist")->toString());
        query.bindValue(":maxAngle", parameters.find("maxAngle")->toString());

        if(!query.exec()){
            db.close();
            QMessageBox msgBox;
            msgBox.setText(query.lastError().text());
            msgBox.exec();
            return false;
        }
    }
    return true;
}

//SELECT * FROM programmDB.Rockets WHERE ID = 3;
