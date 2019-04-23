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

public:
    simulator(QMap<QString, QVariant>* iniParam);

    double current_t,n_y_max,dt;
    QList<double> n_y_sum, n_y, n_z;
    QMap<QString, QList<double>> graphs;
    bool loopOn;



public slots:
    void startSimulate(double Ky, double Kz);
    void targetReached();
    void update();
    void swap();

signals:
    void sendCoordinates(QMap<QString, QVariant>* coord);
    void simulationEnded();
};

#endif // SIMULATOR_H
