#include "simulator.h"

#include <QMap>
#include <QVariant>
#include <QVector>
#include <QtMath>
#include <limits>
#include "math.h"

simulator::simulator(QMap<QString, QVariant> *iniParam):
    params(new QMap<QString, QVariant>(*iniParam)),
    tCSV(0), outputCode(0), current_t(0),
    dt(iniParam->find("Modeling dt")->toDouble()),minDistanceToTarget(std::numeric_limits<double>::max()),MaxAngleOfSight(0),
    n_y_sum(), n_y(), n_z(),
    loopOn(true)

{
    delete iniParam;
}
#include <QDebug>
void simulator::startSimulate(double Ky, double Kz)
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
                    params->find("LA pitch max").value().toDouble(),
                    params->find("LA t delay").value().toDouble(),
                    params->find("LA needed angle").value().toDouble());//deltaAngle LA needed angle

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
                         Ky,Kz,
                         params->find("Rock explode_dist").value().toDouble(),
                         params->find("Modeling sightMaxValue").value().toDouble(),
                         params->find("Modeling rocketAltitude").value().toDouble(),
                         3.6,
                         0.2,
                         0.0314,
                         0.0314,
                         0.476);

    connect(missile,SIGNAL(targetGetReached(int)),
            this, SLOT(swap(int)));

    loopOn = true;

    while (loopOn){
        update();
    }
    targetReached();
}

#include <QDebug>
void simulator::targetReached()
{
    n_y_max = *std::max_element(n_y_sum.begin(), n_y_sum.end());
    Vend = missile->getV();

    graphs.insert("Ny_sum", n_y_sum);
    graphs.insert("N_y", n_y);
    graphs.insert("N_z", n_z);

    delete missile;
    delete target;
    emit simulationEnded();
}

void simulator::swap(int code)
{
    loopOn = !loopOn;
    outputCode = code;
}

void simulator::update()
{
    target->update(dt);
    missile->update(dt);

    tCSV += dt;
    if (fmod(tCSV, 0.1) < 1e-7){
        QList<double> targetCoor    {target->getX(),    target->getY(),     target->getZ()};
        QList<double> LACoor        {missile->getX(),   missile->getY(),    missile->getZ()};
        QMap<QString, QVariant>* csvData = new QMap<QString,QVariant>;
        csvData->insert("Target", QVariant::fromValue(targetCoor));
        csvData->insert("LA", QVariant::fromValue(LACoor));
        emit sendCoordinates(csvData);
        tCSV = 0;
    }

    if (missile->getDistanceToTarget() < 11){
        swap(0);
    }

    n_y_sum.push_back(std::abs(missile->getNy()));
    n_y.push_back(missile->getN_pitch());
    n_z.push_back(missile->getN_roll());
    current_t+=dt;

    if (missile->getDistanceToTarget() < minDistanceToTarget){
        minDistanceToTarget = missile->getDistanceToTarget();
    }

    if (MaxAngleOfSight < missile->getAngleOfSight()){
        MaxAngleOfSight = missile->getAngleOfSight();
    }

    if (current_t > 200){
        swap(1);
    }
}
