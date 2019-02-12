#include "model.h"

#include <QMap>
#include <QVariant>
#include "math.h"

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
    simulator sim(params);

    for (double k(k0); k1-floor(k)>1e-4; k+=dk) {
        for (double n(n0); n1-floor(n)>1e-4; n+=dn) {
            sim.startSimulate(k,n);
        }
    }
}
