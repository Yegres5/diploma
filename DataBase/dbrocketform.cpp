#include "dbrocketform.h"
#include "ui_dbrocketform.h"

#include <QStringList>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QBuffer>
#include <QDebug>
#include <QInputDialog>
#include <QDir>

DBRocketForm::DBRocketForm(QList<QVariant> *dbData, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DBRocketForm)
{
    ui->setupUi(this);
    if (dbData == nullptr){
        DB_Parser = new dataBaseParser();
    }else{
        DB_Parser = new dataBaseParser(dbData);
    }


    QList<QList<QVariant>> lst = DB_Parser->parseCloumns(QStringList({"ID","NAME"}));

    for (auto& s : lst){
        QListWidgetItem* it = new QListWidgetItem(ui->listWidget_Rockets);
        it->setData(Qt::UserRole,QVariant(s));
        QString str(s.last().toString());
        it->setText(s.last().toString());
        ui->listWidget_Rockets->addItem(it);
    }

    QImage image(":/images/No_image_available.png");

    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem((QPixmap::fromImage(image)));
    ui->graphicsView_Image->setScene( scene );
    scene->addItem( item );
    item->setPos(0,0);
    ui->graphicsView_Image->show();

    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::KeepAspectRatio);

    ui->graphicsView_Image->setStyleSheet("background: transparent; border-width: 0px; border-style: solid");

    ui->tableWidget_Parameters->setRowCount(9);
    ui->tableWidget_Parameters->setColumnCount(1);
    ui->tableWidget_Parameters->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Parameters")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(0,new QTableWidgetItem(tr("Name")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(1,new QTableWidgetItem(tr("Length")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(2,new QTableWidgetItem(tr("Diameter")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(3,new QTableWidgetItem(tr("Ae")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(4,new QTableWidgetItem(tr("Sm")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(5,new QTableWidgetItem(tr("Ln")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(6,new QTableWidgetItem(tr("NyMax")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(7,new QTableWidgetItem(tr("ExplodeDistance")));
    ui->tableWidget_Parameters->setVerticalHeaderItem(8,new QTableWidgetItem(tr("MaxAngle")));

    for (int i(0); i<9; i++){
        ui->tableWidget_Parameters->setItem(i,0, new QTableWidgetItem());
    }

    ui->tableWidget_Parameters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

DBRocketForm::~DBRocketForm()
{
    if (ui->listWidget_Rockets->selectedItems().isEmpty()){
        emit sendRocketData(nullptr);
    }else{
        QMap<QString, double>* data = new QMap<QString,double>();
        data->insert("Rock Length", ui->tableWidget_Parameters->item(1,0)->text().toDouble());
        data->insert("Rock Diameter", ui->tableWidget_Parameters->item(2,0)->text().toDouble());
        data->insert("Rock Ae", ui->tableWidget_Parameters->item(3,0)->text().toDouble());
        data->insert("Rock Sm", ui->tableWidget_Parameters->item(4,0)->text().toDouble());
        data->insert("Rock Ln", ui->tableWidget_Parameters->item(5,0)->text().toDouble());
        data->insert("Rock NyMax", ui->tableWidget_Parameters->item(6,0)->text().toDouble());
        data->insert("Rock explode_dist", ui->tableWidget_Parameters->item(7,0)->text().toDouble());
        data->insert("Modeling MaxModelingAngle", ui->tableWidget_Parameters->item(8,0)->text().toDouble());
        emit sendRocketData(data);
    }
    delete DB_Parser;
    delete ui;
}

void DBRocketForm::resizeEvent(QResizeEvent *event)
{
    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::KeepAspectRatio);
    Q_UNUSED(event);
}

void DBRocketForm::showEvent(QShowEvent *event)
{
    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::KeepAspectRatio);
    Q_UNUSED(event);
}

void DBRocketForm::on_listWidget_Rockets_itemPressed(QListWidgetItem *item)
{
    QList<QVariant> vec = DB_Parser->parseCloumns(QStringList({"*"}), item->data(Qt::UserRole).toList().first().toInt()).first();

    QGraphicsPixmapItem* PixItem = new QGraphicsPixmapItem((QPixmap::fromImage(vec.at(2).value<QImage>())));

    QBuffer buffer(&ui->graphicsView_Image->imageFullSize);
    buffer.open(QIODevice::WriteOnly);
    vec.at(2).value<QImage>().save(&buffer, "PNG");
    buffer.close();

    QGraphicsScene* scene = new QGraphicsScene;
    ui->graphicsView_Image->setScene( scene );
    scene->addItem( PixItem );
    PixItem->setPos(0,0);
    ui->graphicsView_Image->show();

//    if (!vec.at(2).isNull()){
//        ui->graphicsView_Image->imageFullSize = vec.at(2).toByteArray();
//        ui->graphicsView_Image->scene()->removeItem(ui->graphicsView_Image->scene()->items().first());
//        ui->graphicsView_Image->scene()->addItem(PixItem);
//    }

    vec.removeAt(2);
    vec.removeAt(0);

    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::KeepAspectRatio);
    int i(0);
    for (auto& s : vec){
        ui->tableWidget_Parameters->item(i,0)->setText(s.toString());
        i++;
    }
}

void DBRocketForm::on_button_Change_Selected_clicked()
{
    QMap<QString, QVariant> data;
    data.insert("name", ui->tableWidget_Parameters->item(0,0)->text());
    data.insert("length", ui->tableWidget_Parameters->item(1,0)->text());
    data.insert("diameter", ui->tableWidget_Parameters->item(2,0)->text());
    data.insert("ae", ui->tableWidget_Parameters->item(3,0)->text());
    data.insert("sm", ui->tableWidget_Parameters->item(4,0)->text());
    data.insert("ln", ui->tableWidget_Parameters->item(5,0)->text());
    data.insert("nymax", ui->tableWidget_Parameters->item(6,0)->text());
    data.insert("explodeDist", ui->tableWidget_Parameters->item(7,0)->text());
    data.insert("maxAngle", ui->tableWidget_Parameters->item(8,0)->text());
    data.insert("image", ui->graphicsView_Image->imageFullSize);
    DB_Parser->changeRocket(data, ui->listWidget_Rockets->selectedItems().first()->data(Qt::UserRole).toList().first().toInt());
}

void DBRocketForm::on_button_Delete_Selected_clicked()
{
    DB_Parser->deleteRow(ui->listWidget_Rockets->selectedItems().first()->data(Qt::UserRole).toList().first().toInt());
    qDeleteAll(ui->listWidget_Rockets->selectedItems());
}

void DBRocketForm::on_button_Create_New_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Rocket name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()){
        QMap<QString, QVariant> data;
        data.insert("name",text);
        data.insert("length", ui->tableWidget_Parameters->item(1,0)->text());
        data.insert("diameter", ui->tableWidget_Parameters->item(2,0)->text());
        data.insert("ae", ui->tableWidget_Parameters->item(3,0)->text());
        data.insert("sm", ui->tableWidget_Parameters->item(4,0)->text());
        data.insert("ln", ui->tableWidget_Parameters->item(5,0)->text());
        data.insert("nymax", ui->tableWidget_Parameters->item(6,0)->text());
        data.insert("explodeDist", ui->tableWidget_Parameters->item(7,0)->text());
        data.insert("maxAngle", ui->tableWidget_Parameters->item(8,0)->text());

        QImage image(":/images/No_image_available.png");

        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        data.insert("image", arr);

        QList<QVariant> pushList;
        int id = DB_Parser->pushNewRocket(data);
        pushList.push_back(id);
        pushList.push_back(text);

        QListWidgetItem* it = new QListWidgetItem(ui->listWidget_Rockets);
        it->setData(Qt::UserRole,QVariant(pushList));
        it->setText(text);
        ui->listWidget_Rockets->addItem(it);
    }
}
