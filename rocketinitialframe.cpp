#include "rocketinitialframe.h"
#include "ui_rocketinitialframe.h"

rocketInitialFrame::rocketInitialFrame(QWidget* parent):
    QFrame (parent),
    ui(new Ui::rocketInitialFrame)
{
    ui->setupUi(this);
}

rocketInitialFrame::~rocketInitialFrame()
{
    delete ui;
}

QMap<QString, double> rocketInitialFrame::getInitialParametrs()
{
    QMap<QString, double> initialMap;
    initialMap.insert("Rock x",ui->LineEdit_X->text().toDouble());
    initialMap.insert("Rock y",ui->LineEdit_Y->text().toDouble());
    initialMap.insert("Rock z",ui->LineEdit_Z->text().toDouble());
    initialMap.insert("Rock v",ui->LineEdit_V->text().toDouble());
    initialMap.insert("Rock teta",ui->LineEdit_teta->text().toDouble());
    initialMap.insert("Rock psi",ui->LineEdit_psi->text().toDouble());
    initialMap.insert("Rock k",ui->LineEdit_K->text().toDouble());
    return  initialMap;
}

void rocketInitialFrame::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->LineEdit_X->setText(QString::number(parametrs.find ("Rock x").value()));
    ui->LineEdit_Y->setText(QString::number(parametrs.find ("Rock y").value()));
    ui->LineEdit_Z->setText(QString::number(parametrs.find ("Rock z").value()));
    ui->LineEdit_V->setText(QString::number(parametrs.find ("Rock v").value()));
    ui->LineEdit_teta->setText(QString::number(parametrs.find ("Rock teta").value()));
    ui->LineEdit_psi->setText(QString::number(parametrs.find ("Rock psi").value()));
    ui->LineEdit_K->setText(QString::number(parametrs.find ("Rock k").value()));
}
