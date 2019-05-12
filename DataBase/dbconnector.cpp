#include "dbconnector.h"
#include "ui_dbconnector.h"

#include "DataBase/databaseparser.h"

dbConnector::dbConnector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dbConnector)
{
    ui->setupUi(this);
    ui->pushButton_connect->setEnabled(false);
}

dbConnector::~dbConnector()
{
    delete ui;
}

void dbConnector::on_pushButton_check_clicked()
{
    dataBaseParser parser(ui->lineEdit_driver->text(),
                          ui->lineEdit_NameDB->text(),
                          ui->lineEdit_Port->text().toInt(),
                          ui->lineEdit_UserName->text(),
                          ui->lineEdit_password->text());
    if (parser.checkConnect()){
        ui->pushButton_check->setStyleSheet("color: rgb(0,190,0)");
        ui->pushButton_connect->setEnabled(true);
    }else{
        ui->pushButton_check->setStyleSheet("color: rgb(255,0,0)");
        ui->pushButton_connect->setEnabled(false);
    }
}


void dbConnector::on_pushButton_connect_clicked()
{
    QList<QVariant>* data = new QList<QVariant>({ui->lineEdit_driver->text(),
                                                 ui->lineEdit_NameDB->text(),
                                                 ui->lineEdit_Port->text().toInt(),
                                                 ui->lineEdit_UserName->text(),
                                                 ui->lineEdit_password->text()});

    emit send_dbParameters(data);
    close();
}
