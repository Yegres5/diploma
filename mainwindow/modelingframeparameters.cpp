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
    initialMap.insert("Modeling sightMaxValue",ui->lineEdit_sightMaxValue->text().toDouble()); //mm
    initialMap.insert("Modeling rocketAltitude",ui->lineEdit_rocketAltitude->text().toDouble());
    initialMap.insert("Modeling countSightAngles",ui->lineEdit_countSightAngles->text().toDouble());
    initialMap.insert("Modeling countSpeedAngles",ui->lineEdit_countSpeedAngles->text().toDouble());
    initialMap.insert("Modeling Ky_0",ui->lineEdit_Ky_0->text().toDouble());
    initialMap.insert("Modeling Ky_1",ui->lineEdit_Ky_1->text().toDouble());
    initialMap.insert("Modeling dKy",ui->lineEdit_dKy->text().toDouble());
    initialMap.insert("Modeling Kz_0",ui->lineEdit_Kz_0->text().toDouble());
    initialMap.insert("Modeling Kz_1",ui->lineEdit_Kz_1->text().toDouble());
    initialMap.insert("Modeling dKz",ui->lineEdit_dKz->text().toDouble());
    return  initialMap;
}

void modelingFrameParameters::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->lineEdit_dt->setText(QString::number(parametrs.find ("Modeling dt").value()));
    ui->lineEdit_Ky_0->setText(QString::number(parametrs.find ("Modeling Ky_0").value()));
    ui->lineEdit_Ky_1->setText(QString::number(parametrs.find ("Modeling Ky_1").value()));
    ui->lineEdit_dKy->setText(QString::number(parametrs.find ("Modeling dKy").value()));
    ui->lineEdit_Kz_0->setText(QString::number(parametrs.find ("Modeling Kz_0").value()));
    ui->lineEdit_Kz_1->setText(QString::number(parametrs.find ("Modeling Kz_1").value()));
    ui->lineEdit_dKz->setText(QString::number(parametrs.find ("Modeling dKz").value()));
    ui->lineEdit_sightMaxValue->setText(QString::number(parametrs.find ("Modeling sightMaxValue").value()));
    ui->lineEdit_rocketAltitude->setText(QString::number(parametrs.find ("Modeling rocketAltitude").value()));
    ui->lineEdit_countSightAngles->setText(QString::number(parametrs.find ("Modeling countSightAngles").value()));
    ui->lineEdit_countSpeedAngles->setText(QString::number(parametrs.find ("Modeling countSpeedAngles").value()));
}
