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

QMap<QString, double> modelingFrameParameters::getInitialParametrs()
{
    QMap<QString, double> initialMap;
    initialMap.insert("Modeling dt",ui->lineEdit_dt->text().toDouble());
    return  initialMap;
}

void modelingFrameParameters::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->lineEdit_dt->setText(QString::number(parametrs.find ("Modeling dt").value()));
}
