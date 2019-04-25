#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QVector>

#include "FlyObjects/la.h"
#include "FlyObjects/rocket.h"
#include <QMap>

class simulator:public QObject
{
    Q_OBJECT
private:
    QMap<QString, QVariant>* params;
    LA* target;
    Rocket* missile;
    double tCSV;
    int outputCode;


public:
    simulator(QMap<QString, QVariant>* iniParam);

    double current_t,n_y_max, dt, minDistanceToTarget, Vend, MaxAngleOfSight;
    QList<double> n_y_sum, n_y, n_z;
    QMap<QString, QList<double>> graphs;
    bool loopOn;
    int getOutputCode(){ return outputCode; }

public slots:
    void startSimulate(double Ky, double Kz);
    void targetReached();
    void update();
    void swap(int code);

signals:
    void sendCoordinates(QMap<QString, QVariant>* coord);
    void simulationEnded();
};

#endif // SIMULATOR_H
