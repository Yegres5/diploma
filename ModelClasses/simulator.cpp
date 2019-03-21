#include "simulator.h"

#include <QMap>
#include <QVariant>
#include <QVector>
#include <QtMath>

simulator::simulator(QMap<QString, QVariant> *iniParam):
    params(iniParam),
    current_t(0),
    dt(iniParam->find("Modeling dt")->toDouble()),
    n_y(new QVector<double>),
    loopOn(true)
{
}

void simulator::startSimulate(double k, double n)
{
    current_t = 0;
    n_y->clear();
    target = new LA(params->find("LA x").value().toDouble(),
                    params->find("LA y").value().toDouble(),
                    params->find("LA z").value().toDouble(),
                    params->find("LA v").value().toDouble(),
                    0,
                    0,
                    params->find("LA teta").value().toDouble(),
                    params->find("LA psi").value().toDouble(),
                    0,
                    n,
                    params->find("LA t manouver").value().toDouble(),
                    params->find("LA t delay").value().toDouble());

    missile = new Rocket(params->find ("Rock x").value().toDouble(),
                         params->find ("Rock y").value().toDouble(),
                         params->find ("Rock z").value().toDouble(),
                         params->find ("Rock v").value().toDouble(),
                         0,
                         0,
                         params->find ("Rock teta").value().toDouble(),
                         params->find ("Rock psi").value().toDouble(),
                         0,
                         target,
                         k);

    connect(missile,SIGNAL(targetGetReached()),
            this, SLOT(swap()));

    loopOn = true;
    while (loopOn){
        update();
    }
    targetReached();
}

void simulator::targetReached()
{
    n_y_max = *std::max_element(n_y->begin(), n_y->end());
    delete missile;
    delete target;
    emit simulationEnded();
}

void simulator::swap()
{
    loopOn = !loopOn;
}

#include <QDebug>
void simulator::update()
{
//    if (missile->getDistanceToTarget() < 50){
//        dt = 0.0001;
//    }
    target->update(dt);
    missile->update(dt);
    n_y->push_back(abs(missile->getNy()));
    current_t+=dt;

    QList<double> targetCoor    {target->getX(),    target->getY(),     target->getZ()};
    QList<double> LACoor        {missile->getX(),   missile->getY(),    missile->getZ()};
    QMap<QString, QVariant>* csvData = new QMap<QString,QVariant>;
    csvData->insert("Target", QVariant::fromValue(targetCoor));
    csvData->insert("LA", QVariant::fromValue(LACoor));
    emit sendCoordinates(csvData);

    if (current_t > 200){
        swap();
    }
}
