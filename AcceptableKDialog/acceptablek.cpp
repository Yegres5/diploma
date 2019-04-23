#include "acceptablek.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QThread>

#include "resultframe.h"
#include "ModelClasses/model.h"

AcceptableK::AcceptableK(QMap<QString, QVariant>* iniParam, QWidget* parent, Qt::WindowFlags f):
    QWidget (parent, f)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    ResultFrame* frame = new ResultFrame();
    mainLayout->addWidget(frame,0,0);

    Model* simulationModel = new Model(iniParam);

    QThread *thread= new QThread();
    simulationModel->moveToThread(thread);
    thread->start();

    connect(frame, SIGNAL(startSimulation()),
            simulationModel, SLOT(StartModeling()));

    connect(frame, SIGNAL(startSimulationFor(QList<double>, QList<double>, double)),
            simulationModel, SLOT(StartModelingFor(QList<double>, QList<double>, double)), Qt::DirectConnection);

    connect(frame, SIGNAL(hideDialog()),
            this,  SLOT(hideDia()));
    connect(frame, SIGNAL(showDialog()),
            this,  SLOT(showDia()));

    connect(simulationModel, SIGNAL(sendData(QMap<QString,double>*, QMap<QString,QList<double>>*)),
            frame, SLOT(pasteData(QMap<QString,double>*, QMap<QString,QList<double>>*)));
}

AcceptableK::~AcceptableK()
{
}

void AcceptableK::hideDia()
{
    hide();
}

void AcceptableK::showDia()
{
    show();
}
