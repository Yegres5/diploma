#include "modelingframeparameters.h"
#include "ui_modelingframeparameters.h"

modelingFrameParameters::modelingFrameParameters(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::modelingFrameParameters)
{
    ui->setupUi(this);
}

modelingFrameParameters::~modelingFrameParameters()
{
    delete ui;
}
