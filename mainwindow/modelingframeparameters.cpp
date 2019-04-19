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
    initialMap.insert("Modeling sightMaxValue",ui->lineEdit_sightMaxValue->text().toDouble());
    initialMap.insert("Modeling rocketAltitude",ui->lineEdit_rocketAltitude->text().toDouble());
    initialMap.insert("Modeling countSightAngles",ui->lineEdit_countSightAngles->text().toDouble());
    initialMap.insert("Modeling countSpeedAngles",ui->lineEdit_countSpeedAngles->text().toDouble());
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
    ui->lineEdit_sightMaxValue->setText(QString::number(parametrs.find ("Modeling sightMaxValue").value()));
    ui->lineEdit_rocketAltitude->setText(QString::number(parametrs.find ("Modeling rocketAltitude").value()));
    ui->lineEdit_countSightAngles->setText(QString::number(parametrs.find ("Modeling countSightAngles").value()));
    ui->lineEdit_countSpeedAngles->setText(QString::number(parametrs.find ("Modeling countSpeedAngles").value()));
}
