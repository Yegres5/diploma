#include "simulator.h"

#include <QMap>
#include <QVariant>
#include <QVector>
#include <QtMath>

simulator::simulator(QMap<QString, QVariant> *iniParam):
    params(new QMap<QString, QVariant>(*iniParam)),
    current_t(0),
    dt(iniParam->find("Modeling dt")->toDouble()),
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
                         Ky,Kz);

    connect(missile,SIGNAL(targetGetReached()),
            this, SLOT(swap()));

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
    qDebug() << Q_FUNC_INFO << " V_end = " << missile->getV();

    graphs.insert("Ny_sum", n_y_sum);
    graphs.insert("N_y", n_y);
    graphs.insert("N_z", n_z);

    delete missile;
    delete target;
    emit simulationEnded();
}

void simulator::swap()
{
    loopOn = !loopOn;
}

void simulator::update()
{
    target->update(dt);
    missile->update(dt);
    n_y_sum.push_back(std::abs(missile->getNy()));
    n_y.push_back(missile->getN_pitch());
    n_z.push_back(missile->getN_roll());
    current_t+=dt;

    //qDebug() << Q_FUNC_INFO << " current time = " << current_t;

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
