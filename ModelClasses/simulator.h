#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>

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

public slots:
    void startSimulate(double k, double n);
    void targetReached();
signals:
    void simulationEnded(double T,double n_y_max, QVector<double> n_y, double dt);
};

#endif // SIMULATOR_H
