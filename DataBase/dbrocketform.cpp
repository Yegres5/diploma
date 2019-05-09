#include "dbrocketform.h"
#include "ui_dbrocketform.h"

#include <QStringList>
#include <QGraphicsPixmapItem>
#include <QObject>

DBRocketForm::DBRocketForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DBRocketForm),
    DB_Parser()
{
    ui->setupUi(this);
    QList<QList<QVariant>> lst = DB_Parser.parseCloumns(QStringList({"ID","NAME"}));

    for (auto& s : lst){
        QListWidgetItem* it = new QListWidgetItem(ui->listWidget_Rockets);

        it->setData(Qt::UserRole,QVariant(s));
        QString str(s.last().toString());
        it->setText(s.last().toString());
        ui->listWidget_Rockets->addItem(it);
    }

    QImage image(":/images/No_image_available.png");

    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem((QPixmap::fromImage(image)));
    ui->graphicsView_Image->setScene( scene );
    scene->addItem( item );
    item->setPos(0,0);
    ui->graphicsView_Image->show();

    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::IgnoreAspectRatio);

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

    ui->tableWidget_Parameters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

DBRocketForm::~DBRocketForm()
{
    delete ui;
}

void DBRocketForm::resizeEvent(QResizeEvent *event)
{
    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::IgnoreAspectRatio);
    Q_UNUSED(event);
}

void DBRocketForm::showEvent(QShowEvent *event)
{
    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::IgnoreAspectRatio);
    Q_UNUSED(event);
}

void DBRocketForm::on_listWidget_Rockets_itemPressed(QListWidgetItem *item)
{
    QList<QVariant> vec = DB_Parser.parseCloumns(QStringList({"*"}), item->data(Qt::UserRole).toList().first().toInt()).first();

    QGraphicsPixmapItem* PixItem = new QGraphicsPixmapItem((QPixmap::fromImage(vec.at(2).value<QImage>())));

    if (!vec.at(2).isNull()){
        ui->graphicsView_Image->scene()->removeItem(ui->graphicsView_Image->scene()->items().first());
        ui->graphicsView_Image->scene()->addItem(PixItem);
    }

    vec.removeAt(2);
    vec.removeAt(0);

    ui->graphicsView_Image->fitInView(ui->graphicsView_Image->scene()->sceneRect(), Qt::IgnoreAspectRatio);
    int i(0);
    for (auto& s : vec){
        ui->tableWidget_Parameters->setItem(0,i,new QTableWidgetItem(s.toString()));
        i++;
    }
}
#include <QDebug>
#include <QBuffer>
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
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    ui->graphicsView_Image->grab().toImage().save(&buffer, "PNG");
    data.insert("image", arr);
    DB_Parser.changeRocket(data, ui->listWidget_Rockets->selectedItems().first()->data(Qt::UserRole).toList().first().toInt());



}

void DBRocketForm::on_graphicsView_Image_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "req";
}
