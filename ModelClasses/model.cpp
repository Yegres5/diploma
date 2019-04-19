#include "model.h"

#include <QMap>
#include <QVariant>
#include <math.h>
#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QDebug>

#include "simulator.h"

Model::Model(QMap<QString, QVariant> *iniParam):
    params(iniParam)
{
    const QStringList str({
                      "LA v",
                      "LA pitch max",
                      "LA t delay",
                      "Rock x",
                      "Rock y",
                      "Rock z",
                      "Rock v",
                      "Rock teta",
                      "Modeling dt"});

    for (auto& it : str){
        if(params->contains(it)){
            staticParams.insert(it,*params->find(it));
        }
    }
    staticParams.insert("LA teta", 0);
    staticParams.insert("Rock psi",0);
}

struct pairOfAngles{
    double delta;
    double lambda;
    pairOfAngles(double delta, double lambda):delta(delta/180*M_PI),lambda(lambda/180*M_PI){}
};

void Model::StartModeling()
{
    //QMap<double, QList<double>> possibleLambda;    // delta, QList<double> lambda
    QList<pairOfAngles> possibleAngleValues;

    for (double delta(0);
         delta <= ((*params->find("Modeling sightMaxValue")).toDouble());
         delta += ((*params->find("Modeling sightMaxValue")).toDouble())/
         ((*params->find("Modeling countSightAngles")).toDouble() - 1))
    {
        QList<double> lambdas;
        for (double lambda(0);
             lambda <= (180 - delta);
             lambda += (180 - delta)/((*params->find("Modeling countSpeedAngles")).toDouble() - 1))
        {
            pairOfAngles pair(delta,lambda);
            possibleAngleValues.push_back(pair);
        }
    }

    for (double k((*params->find("Modeling K0")).toDouble());
                    (*params->find("Modeling K1")).toDouble() >= k ;
                    k+=(*params->find("Modeling dK")).toDouble()) {
        for (auto& angles : possibleAngleValues) {
            QMap<QString, QVariant> dynamicData;

            double a = sqrt(pow((*params->find("Rock MeshD")).toDouble(), 2) - pow((*params->find("LA dH")).toDouble(), 2));
            dynamicData.insert("LA x", a*cos(angles.delta));
            dynamicData.insert("LA y", (*params->find("LA dH")).toDouble());
            dynamicData.insert("LA z", a*sin(angles.delta));
            dynamicData.insert("LA psi", angles.lambda);
            dynamicData.insert("Rocket k", k);
            qDebug() << Q_FUNC_INFO << "LOOP " << k;

            QMap<QString, QVariant> data(staticParams);

            for(auto& it : dynamicData.keys()){
                data.insert(it,(*dynamicData.find(it)));
            }


            simulator* sim = new simulator(&data);
            connect(this, SIGNAL(startSimulate(double)),
                    sim, SLOT(startSimulate(double)));

            clearCSVFiles();
            connect(sim, SIGNAL(sendCoordinates(QMap<QString,  QVariant>*)),
                    this, SLOT(writeCoordToCSV(QMap<QString, QVariant>*)));

            emit startSimulate(k);
            emit sendData(k, 0, sim->current_t, sim->dt, sim->n_y_max, sim->n_y);
            delete sim;
        }

    }
}

void Model::StartModelingFor(double K, double N, double dt)
{
    qDebug() << Q_FUNC_INFO;
    clearCSVFiles();

    QMap<QString,QVariant> tempMap(*params);

    if (dt > 1e-7){
        tempMap["Modeling dt"] = dt;
    }

    simulator* sim = new simulator(&tempMap);
    connect(this, SIGNAL(startSimulate(double, double)),
            sim, SLOT(startSimulate(double, double)), Qt::DirectConnection);

    connect(sim, SIGNAL(sendCoordinates(QMap<QString,  QVariant>*)),
            this, SLOT(writeCoordToCSV(QMap<QString, QVariant>*)));

    qDebug() <<Q_FUNC_INFO <<  "K = " << K << "N_y = " << N;
    emit startSimulate(K);

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

        QString filename = obj.value("pythonScriptPath").toString() + "rocket.csv";
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

        QString filename = obj.value("pythonScriptPath").toString() + "target.csv";
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

    QFile file(obj.value("pythonScriptPath").toString() + "rocket.csv");
    file.resize(0);
    file.setFileName(obj.value("pythonScriptPath").toString() + "target.csv");
    file.resize(0);
}
