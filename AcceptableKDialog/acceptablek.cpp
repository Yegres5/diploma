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
    QLabel* label = new QLabel("Title");
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainLayout->addWidget(label,0,0);
    ResultFrame* frame = new ResultFrame();
    mainLayout->addWidget(frame,1,0);

    Model* simulationModel = new Model(iniParam);

//    QThread *thread= new QThread();
//    simulationModel->moveToThread(thread);
//    thread->start();

    connect(frame, SIGNAL(startSimulation()),
            simulationModel, SLOT(StartModeling()));

    connect(frame, SIGNAL(startSimulationFor(double, double, double)),
            simulationModel, SLOT(StartModelingFor(double, double, double)));

    connect(frame, SIGNAL(hideDialog()),
            this,  SLOT(hideDia()));
    connect(frame, SIGNAL(showDialog()),
            this,  SLOT(showDia()));

    connect(simulationModel, SIGNAL(sendData(double, double, double, double, double, QVector<double>*)),
            frame, SLOT(pasteData(double, double, double, double, double, QVector<double>*)));
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
