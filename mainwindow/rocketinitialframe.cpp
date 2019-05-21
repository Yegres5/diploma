#include "rocketinitialframe.h"
#include "ui_rocketinitialframe.h"

#include <QGridLayout>
#include <QMessageBox>

#include "DataBase/dbrocketform.h"

rocketInitialFrame::rocketInitialFrame(QWidget* parent):
    QFrame (parent),
    ui(new Ui::rocketInitialFrame),
    dbIniParameters(nullptr),
    rocketData(nullptr)
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
    initialMap.insert("Rock dH",-(ui->LineEdit_dH->text().toDouble()));

    if (rocketData != nullptr){
        for (auto& k : rocketData->keys()){
            initialMap.insert(k, *rocketData->find(k));
        }
    }

    return  initialMap;
}

void rocketInitialFrame::loadInitialParametrs(QMap<QString, double> parametrs)
{
    ui->LineEdit_V->setText(QString::number(parametrs.find ("Rock v").value()));
    ui->LineEdit_teta->setText(QString::number(parametrs.find ("Rock teta").value()));
    ui->LineEdit_MeshD->setText(QString::number(parametrs.find("Rock MeshD").value()));
    ui->LineEdit_dH->setText(QString::number(-(parametrs.find("Rock dH").value())));
}

void rocketInitialFrame::on_pushButton_clicked()
{
    if (dbIniParameters == nullptr){
        QMessageBox msgBox;
        msgBox.setText(tr("Data base is not connected."));
        msgBox.exec();
        return;
    }
    QWidget* wgt = new QWidget();
    wgt->setAttribute(Qt::WA_DeleteOnClose);

    QGridLayout* mainLayout = new QGridLayout(wgt);

    DBRocketForm* form = new DBRocketForm(dbIniParameters, wgt);

    mainLayout->addWidget(form,0,0);
    wgt->show();

    connect(form, SIGNAL(sendRocketData(QMap<QString, double>*)),
            this, SLOT(setRocketData(QMap<QString, double>*)));
}

void rocketInitialFrame::setRocketData(QMap<QString, double> *data)
{
    if (rocketData != nullptr){
        delete rocketData;
    }
    if (data != nullptr){
        rocketData = data;
        emit setModelingButtonOn();
    }
}

void rocketInitialFrame::setDBParameters(QList<QVariant> *data){
    dbIniParameters = data;
}
