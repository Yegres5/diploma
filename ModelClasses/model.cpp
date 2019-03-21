#include "model.h"

#include <QMap>
#include <QVariant>
#include <math.h>
#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

#include "simulator.h"

Model::Model(QMap<QString, QVariant> *iniParam):
    n0(iniParam->find ("LA pitch min").value().toDouble()),
    n1(iniParam->find ("LA pitch max").value().toDouble()),
    dn(iniParam->find ("LA pitch delta").value().toDouble()),
    k0(iniParam->find ("Rock K0").value().toDouble()),
    k1(iniParam->find ("Rock K1").value().toDouble()),
    dk(iniParam->find ("Rock dK").value().toDouble()),
    params(iniParam)
{

}
#include <QDebug>

void Model::StartModeling()
{
    simulator* sim = new simulator(params);
    connect(this, SIGNAL(startSimulate(double, double)),
            sim, SLOT(startSimulate(double, double)), Qt::DirectConnection);

    for (double k(k0); k1-k+dk>1e-4; k+=dk) {
        for (double n(n0); n1-n+dn>1e-4; n+=dn) {
            qDebug() << Q_FUNC_INFO <<  "K = " << k << "N_y = " << n;
            emit startSimulate(k,n);
            emit sendData(k, n, sim->current_t, sim->dt, sim->n_y_max, sim->n_y);
        }
    }
    delete sim;
}

void Model::StartModelingFor(double K, double N, double dt)
{
    qDebug() << Q_FUNC_INFO;
    clearCSVFiles();
    QMap<QString,QVariant> tempMap(*params);
    tempMap["Modeling dt"] = dt;
    simulator* sim = new simulator(&tempMap);
    connect(this, SIGNAL(startSimulate(double, double)),
            sim, SLOT(startSimulate(double, double)), Qt::DirectConnection);

    connect(sim, SIGNAL(sendCoordinates(QMap<QString,  QVariant>*)),
            this, SLOT(writeCoordToCSV(QMap<QString, QVariant>*)));

    qDebug() << Q_FUNC_INFO <<  "K = " << K << "N_y = " << N;
    emit startSimulate(K,N);

    delete sim;
}

void Model::writeCoordToCSV(QMap<QString, QVariant>* coord)
{
    if(coord->contains("LA")){
        QList<double> coord_vec = qvariant_cast<QList<double>>(*coord->find("LA"));

        QFile jsonFile(":/JSON/paths.json");
        jsonFile.open(QFile::ReadOnly);

        QJsonDocument doc (QJsonDocument().fromJson(jsonFile.readAll()));
        QJsonObject obj (doc.object());

        QString filename = obj.value("rocketPath").toString();
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&file);
            stream << coord_vec.at(0) << "," << coord_vec.at(1) << "," << coord_vec.at(2) << endl;
        }
        file.close();
    }

    if(coord->contains("Target")){
        QList<double> coord_vec = qvariant_cast<QList<double>>(*coord->find("Target"));

        QFile jsonFile(":/JSON/paths.json");
        jsonFile.open(QFile::ReadOnly);

        QJsonDocument doc (QJsonDocument().fromJson(jsonFile.readAll()));
        QJsonObject obj (doc.object());

        QString filename = obj.value("targetPath").toString();
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&file);
            stream << coord_vec.at(0) << "," << coord_vec.at(1) << "," << coord_vec.at(2) << endl;
        }
        file.close();
    }
    delete coord;
}

void Model::clearCSVFiles()
{
    QFile jsonFile(":/JSON/paths.json");
    jsonFile.open(QFile::ReadOnly);

    QJsonDocument doc (QJsonDocument().fromJson(jsonFile.readAll()));
    QJsonObject obj (doc.object());
    QStringList arguments { obj.value("pythonScriptPath").toString() };

    QFile file(obj.value("rocketPath").toString());
    file.resize(0);
    file.setFileName(obj.value("targetPath").toString());
    file.resize(0);
}
