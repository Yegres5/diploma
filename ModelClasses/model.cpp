#include "model.h"

#include <QMap>
#include <QVariant>
#include <math.h>
#include <QEventLoop>

#include "simulator.h"

Model::Model(QMap<QString, QVariant> *iniParam):
    n0(iniParam->find ("LA pitch min").value().toDouble()),
    n1(iniParam->find ("LA pitch max").value().toDouble()),
    dn(iniParam->find ("LA pitch delta").value().toDouble()),
    k0(iniParam->find ("Rock K0").value().toDouble()),
    k1(iniParam->find ("Rock K1").value().toDouble()),
    dk(iniParam->find ("Rock dK").value().toDouble()),
    params(iniParam)
{

}
#include <QDebug>

void Model::StartModeling()
{
    simulator* sim = new simulator(params);
    connect(this, SIGNAL(startSimulate(double, double)),
            sim, SLOT(startSimulate(double, double)), Qt::DirectConnection);

    for (double k(k0); k1-k+dk>1e-4; k+=dk) {
        for (double n(n0); n1-n+dn>1e-4; n+=dn) {
            qDebug() << Q_FUNC_INFO <<  "K = " << k << "N_y = " << n;
            emit startSimulate(k,n);
            emit sendData(k, n, sim->current_t, sim->dt, sim->n_y_max, sim->n_y);
        }
    }
    delete sim;
}
