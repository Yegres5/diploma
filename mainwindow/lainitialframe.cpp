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
    initialMap.insert("LA v",ui->LineEdit_V->text().toDouble());
    initialMap.insert("LA pitch max",ui->LineEdit_n_pitch_max->text().toDouble());
    initialMap.insert("LA t delay",ui->LineEdit_t_manouver_delay->text().toDouble());
    return  initialMap;
}

void laInitialFrame::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->LineEdit_V->setText(QString::number(parametrs.find ("LA v").value()));
    ui->LineEdit_n_pitch_max->setText(QString::number(parametrs.find ("LA pitch max").value()));
    ui->LineEdit_t_manouver_delay->setText(QString::number(parametrs.find ("LA t delay").value()));
}
