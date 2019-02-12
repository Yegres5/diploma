#include "acceptablek.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include "resultframe.h"
#include "ModelClasses/model.h"

AcceptableK::AcceptableK(QMap<QString, QVariant>* iniParam, QWidget* parent):
    QDialog (parent)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    QLabel* label = new QLabel("Title");
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainLayout->addWidget(label,0,0);
    ResultFrame* frame = new ResultFrame();
    mainLayout->addWidget(frame,1,0);

    Model simulationModel(iniParam);
    simulationModel.StartModeling();
}

AcceptableK::~AcceptableK()
{
}
