#include "lainitialframe.h"
#include "ui_lainitialframe.h"

laInitialFrame::laInitialFrame(QWidget *parent):
    QFrame(parent),
    ui(new Ui::laInitialFrame)
{
    ui->setupUi(this);
}

laInitialFrame::~laInitialFrame()
{
    delete ui;
}

QMap<QString, double> laInitialFrame::getInitialParametrs()
{
    QMap<QString, double> initialMap;
    initialMap.insert("LA x",ui->LineEdit_X->text().toDouble());
    initialMap.insert("LA y",ui->LineEdit_Y->text().toDouble());
    initialMap.insert("LA z",ui->LineEdit_Z->text().toDouble());
    initialMap.insert("LA v",ui->LineEdit_V->text().toDouble());
    initialMap.insert("LA teta",ui->LineEdit_teta->text().toDouble());
    initialMap.insert("LA psi",ui->LineEdit_psi->text().toDouble());
    return  initialMap;
}

void laInitialFrame::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->LineEdit_X->setText(QString::number(parametrs.find ("LA x").value()));
    ui->LineEdit_Y->setText(QString::number(parametrs.find ("LA y").value()));
    ui->LineEdit_Z->setText(QString::number(parametrs.find ("LA z").value()));
    ui->LineEdit_V->setText(QString::number(parametrs.find ("LA v").value()));
    ui->LineEdit_teta->setText(QString::number(parametrs.find ("LA teta").value()));
    ui->LineEdit_psi->setText(QString::number(parametrs.find ("LA psi").value()));
}
