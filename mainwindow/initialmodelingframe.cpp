#include "initialmodelingframe.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QVector>
#include <QVariant>

#include "lainitialframe.h"
#include "rocketinitialframe.h"
#include "modelingframeparameters.h"

InitialModelingFrame::InitialModelingFrame(QWidget *parent):
    QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this) ;
    layout->setMargin(9);

    QGridLayout *field_layout = new QGridLayout(nullptr) ;
    field_layout->setMargin(0) ;
    layout->addLayout(field_layout) ;

    m_laFrame = new laInitialFrame(this);
    field_layout->addWidget(m_laFrame,0,0,1,1);

    m_rocketFrame = new rocketInitialFrame(this);
    field_layout->addWidget(m_rocketFrame,0,1,1,1);

    m_modelFrame = new modelingFrameParameters(this);
    field_layout->addWidget(m_modelFrame,1,0,1,2);

    m_StartModelingButton = new QPushButton("StartModeling",this);
    field_layout->addWidget(m_StartModelingButton,2,0,1,2);

    connect(m_StartModelingButton, SIGNAL(clicked()),
            this, SLOT(StartModeling()));
}

QMap<QString, QVariant> InitialModelingFrame::getInitialParametrs()
{
    QMap<QString, QVariant> iniMap;
    QVector<QMap<QString, double>> mapVector;

    mapVector.push_back(m_laFrame->getInitialParametrs());
    mapVector.push_back(m_rocketFrame->getInitialParametrs());
    mapVector.push_back(m_modelFrame->getInitialParametrs());

    for (QVector<QMap<QString, double>>::iterator dataMap(mapVector.begin()); dataMap!=mapVector.end(); dataMap++){
        QMap<QString, double> map(*dataMap);
        for (QMap<QString, double>::iterator iter(map.begin());
             iter!=map.end();
             iter++){
            iniMap.insert(iter.key(),iter.value());
        }
    }
    return iniMap;
}

void InitialModelingFrame::setInitialParametrs(QMap<QString, QVariant> parametrs)
{
    QMap<QString, double> laParam;
    QMap<QString, double> rockParam;
    QMap<QString, double> modelParam;
    for (QMap<QString, QVariant>::iterator iter(parametrs.begin()); iter!=parametrs.end();iter++) {
        if (iter.key().contains("LA")){
            laParam.insert(iter.key(),iter.value().toDouble());
        }
        if (iter.key().contains("Rock")){
            rockParam.insert(iter.key(),iter.value().toDouble());
        }
        if (iter.key().contains("Modeling")){
            modelParam.insert(iter.key(),iter.value().toDouble());
        }
    }
    m_laFrame->loadInitialParametrs(laParam);
    m_rocketFrame->loadInitialParametrs(rockParam);
    m_modelFrame->loadInitialParametrs(modelParam);
}

void InitialModelingFrame::StartModeling()
{
    QMap<QString, QVariant>* map = new QMap<QString, QVariant>(getInitialParametrs());
    emit startModelingPressed(map);
}
