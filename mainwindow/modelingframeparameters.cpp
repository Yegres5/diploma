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
    initialMap.insert("Modeling K0",ui->lineEdit_K0->text().toDouble());
    initialMap.insert("Modeling K1",ui->lineEdit_K1->text().toDouble());
    initialMap.insert("Modeling dK",ui->lineEdit_dK->text().toDouble());
    return  initialMap;
}

void modelingFrameParameters::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->lineEdit_dt->setText(QString::number(parametrs.find ("Modeling dt").value()));
    ui->lineEdit_K0->setText(QString::number(parametrs.find ("Modeling K0").value()));
    ui->lineEdit_K1->setText(QString::number(parametrs.find ("Modeling K1").value()));
    ui->lineEdit_dK->setText(QString::number(parametrs.find ("Modeling dK").value()));
}
