#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QVector>

#include "FlyObjects/la.h"
#include "FlyObjects/rocket.h"

class simulator:public QObject
{
    Q_OBJECT
private:
    QMap<QString, QVariant>* params;
    LA* target;
    Rocket* missile;

public:
    simulator(QMap<QString, QVariant>* iniParam);

    double current_t,n_y_max,dt;
    QVector<double>* n_y;
    bool loopOn;


public slots:
    void startSimulate(double k, double n);
    void targetReached();
    void update();
    void swap();

signals:
    void sendCoordinates(QMap<QString, QVariant>* coord);
    void simulationEnded();
};

#endif // SIMULATOR_H
