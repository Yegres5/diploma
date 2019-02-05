#include "initialmodelingframe.h"

#include <QGridLayout>
#include <QPushButton>

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

void InitialModelingFrame::StartModeling()
{
}
