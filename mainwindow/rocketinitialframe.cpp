#include "rocketinitialframe.h"
#include "ui_rocketinitialframe.h"

#include <QGridLayout>

#include "DataBase/dbrocketform.h"

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
    initialMap.insert("Rock v",ui->LineEdit_V->text().toDouble());
    initialMap.insert("Rock teta",ui->LineEdit_teta->text().toDouble());
    initialMap.insert("Rock MeshD",ui->LineEdit_MeshD->text().toDouble());
    initialMap.insert("Rock explode_dist",ui->LineEdit_explode_dist->text().toDouble());
    return  initialMap;
}

void rocketInitialFrame::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->LineEdit_V->setText(QString::number(parametrs.find ("Rock v").value()));
    ui->LineEdit_teta->setText(QString::number(parametrs.find ("Rock teta").value()));
    ui->LineEdit_MeshD->setText(QString::number(parametrs.find("Rock MeshD").value()));
    ui->LineEdit_explode_dist->setText(QString::number(parametrs.find ("Rock explode_dist").value()));
}

void rocketInitialFrame::on_pushButton_clicked()
{


    QWidget* wgt = new QWidget();


    QGridLayout* mainLayout = new QGridLayout(wgt);
    DBRocketForm* form = new DBRocketForm();
    mainLayout->addWidget(form,0,0);
    wgt->show();
//    wgt->show();
//    form->show();
    //lay->addItem(form,0,0);
}
