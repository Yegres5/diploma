#include "simulator.h"

#include <QMap>
#include <QVariant>
#include <QVector>

simulator::simulator(QMap<QString, QVariant> *iniParam):
    params(iniParam),
    current_t(0),
    dt(iniParam->find("Modeling dt")->toDouble()),
    n_y(new QVector<double>)
{
    connect(this, SIGNAL(loop()),
            this, SLOT(update()));
}

void simulator::startSimulate(double k, double n)
{
    current_t = 0;
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

    emit loop();
}

void simulator::targetReached()
{
    n_y_max = missile->n_y_max;
    delete missile;
    delete target;
    disconnect(this, SIGNAL(loop()),
               this, SLOT(targetReached()));
    connect(this, SIGNAL(loop()),
            this, SLOT(update()));
    emit simulationEnded();
}

void simulator::swap()
{
    disconnect(this, SIGNAL(loop()),
               this, SLOT(update()));
    connect(this, SIGNAL(loop()),
            this, SLOT(targetReached()));
}
#include <QDebug>
void simulator::update()
{
    qDebug() << current_t;
    target->update(dt);
    missile->update(dt);
    n_y->push_back(missile->getNy());
    current_t+=dt;
    emit loop();
}
