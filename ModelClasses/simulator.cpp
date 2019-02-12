#include "simulator.h"

#include <QMap>
#include <QVariant>
#include <QVector>

simulator::simulator(QMap<QString, QVariant> *iniParam):
    params(iniParam)
{
}

void simulator::startSimulate(double k, double n)
{
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
            this, SLOT(targetReached()));
}

void simulator::targetReached()
{
    delete missile;
    delete target;
    emit simulationEnded(0,0,0,0);
}

