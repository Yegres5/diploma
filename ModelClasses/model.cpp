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

Model::Model(QMap<QString, QVariant> *iniParam, QObject *parent):
    QObject(parent),
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
                      "Rock explode_dist",
                      "Modeling dt",
                      "Modeling sightMaxValue",
                      "Modeling MaxModelingAngle",
                      "Modeling rocketAltitude",
                      "Rock Length",
                      "Rock Diameter",
                      "Rock Ae",
                      "Rock Sm",
                      "Rock Ln",
                      "Rock NyMax"});

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

    for (double Ky((*params->find("Modeling Ky_0")).toDouble());
                    (*params->find("Modeling Ky_1")).toDouble() >= Ky ;
                    Ky+=(*params->find("Modeling dKy")).toDouble()) {

        for (double Kz((*params->find("Modeling Kz_0")).toDouble());
                        (*params->find("Modeling Kz_1")).toDouble() >= Kz ;
                        Kz+=(*params->find("Modeling dKz")).toDouble()) {

            for (auto& angles : possibleAngleValues) {
                QMap<QString, QVariant> dynamicData;

                double a = sqrt(pow((*params->find("Rock MeshD")).toDouble(), 2) - pow((*params->find("LA dH")).toDouble(), 2));
                dynamicData.insert("LA x", a*cos(angles.delta));
                dynamicData.insert("LA y", (*params->find("LA dH")).toDouble());
                dynamicData.insert("LA z", a*sin(angles.delta));
                dynamicData.insert("LA psi", angles.lambda);
                dynamicData.insert("Rocket ky", Ky);
                dynamicData.insert("Rocket kz", Kz);
                dynamicData.insert("LA needed angle", 2*M_PI-angles.delta);
                qDebug() << Q_FUNC_INFO << "LOOP " << Ky << "___" << Kz;

                QMap<QString, QVariant> data(staticParams);

                for(auto& it : dynamicData.keys()){
                    data.insert(it,(*dynamicData.find(it)));
                }

                simulator* sim = new simulator(new QMap<QString,QVariant>(data));
                connect(this, SIGNAL(startSimulate(double, double)),
                        sim, SLOT(startSimulate(double, double)));

                emit startSimulate(Ky,Kz);

                QMap<QString, double> tempMap;
                for (auto& it : data.keys()){
                    tempMap.insert(it, (*data.find(it)).toDouble());
                }
                tempMap.insert("Rock Ky",Ky);
                tempMap.insert("Rock Kz",Kz);
                tempMap.insert("LA delta",angles.delta);
                tempMap.insert("LA lambda", angles.lambda);
                tempMap.insert("Simulator t",sim->current_t);
                tempMap.insert("Simulator dt",sim->dt);
                tempMap.insert("Simulator n_y_max",sim->n_y_max);
                tempMap.insert("Error message", sim->getOutputCode());
                tempMap.insert("V_end", sim->Vend);
                tempMap.insert("Min distance to target", sim->minDistanceToTarget);
                tempMap.insert("Max overload", sim->n_y_max);
                tempMap.insert("Max angle of sight", sim->MaxAngleOfSight);

                QMap<QString, QList<double>>* graphMap(new QMap<QString, QList<double>>(sim->graphs));

                emit sendData(new QMap<QString,double>(tempMap), graphMap);
                delete sim;
            }
        }
    }
}


void Model::StartModelingFor(QList<double> K, QList<double> angles)
{
    qDebug() << Q_FUNC_INFO;
    clearCSVFiles();

    QMap<QString,QVariant> tempMap(staticParams);

    QMap<QString,double> dynamicData;
    double a = sqrt(pow((*params->find("Rock MeshD")).toDouble(), 2) - pow((*params->find("LA dH")).toDouble(), 2));
    dynamicData.insert("LA x", a*cos(angles.first()*M_PI/180));
    dynamicData.insert("LA y", (*params->find("LA dH")).toDouble());
    dynamicData.insert("LA z", a*sin(angles.first()*M_PI/180));
    dynamicData.insert("LA psi", angles.last()/180*M_PI);
    dynamicData.insert("Rocket Ky", K.first());
    dynamicData.insert("Rocket Kz", K.last());
    dynamicData.insert("LA needed angle", 2*M_PI-angles.first()*M_PI/180);

    QMap<QString, QVariant> data(staticParams);

    for(auto& it : dynamicData.keys()){
        data.insert(it,(*dynamicData.find(it)));
    }

    simulator* sim = new simulator(new QMap<QString, QVariant>(data));
    connect(this, SIGNAL(startSimulate(double, double)),
            sim, SLOT(startSimulate(double, double)), Qt::DirectConnection);

    connect(sim, SIGNAL(sendCoordinates(QMap<QString,  QVariant>*)),
            this, SLOT(writeCoordToCSV(QMap<QString, QVariant>*)));

    qDebug() <<Q_FUNC_INFO <<  "K = " << K << "N_y = " << angles;
    qDebug() << Q_FUNC_INFO << "simulate start";
    emit startSimulate(K.first(),K.last());

    delete sim;
    qDebug() << Q_FUNC_INFO << "simulate deleted";
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
