#include "resultframe.h"
#include "ui_resultframe.h"

#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>
#include <math.h>
#include <QString>

#include "tabledrawingdata.h"
#include "headeritem.h"
#include "tabledelegate.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>

ResultFrame::ResultFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ResultFrame)
{
    ui->setupUi(this);
    connect(ui->button_startSimulate,SIGNAL(clicked()),
            this, SIGNAL(startSimulation()));
    connect(ui->button_draw_ny,SIGNAL(clicked()),
            this, SLOT(drawNy()));

    tableDelegate* delegate = new tableDelegate(ui->table_results);
    ui->table_results->setItemDelegate(delegate);
    ui->table_results->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_results->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_results->setMinimumSize(600,400);
}

ResultFrame::~ResultFrame()
{
    delete ui;
}

void ResultFrame::pasteData(double k, double n, double t, double dt, double n_y_max, QVector<double>* n_y)
{
    int headerNum(-1),rowNum(-1);
    for(int i = 0; i < ui->table_results->model()->columnCount(); i++)
    {
        if (abs(ui->table_results->model()->headerData(i, Qt::Horizontal,Qt::UserRole).toDouble() - k) < 1e-4){
            headerNum = i;
            break;
        }
    }
    if (headerNum == -1){
        ui->table_results->setColumnCount(ui->table_results->model()->columnCount()+1);
        headerNum = ui->table_results->model()->columnCount()-1;

        QString name("K = " + QString::number(k));
        QTableWidgetItem* item = new QTableWidgetItem(name);
        QVariant data = k;
        item->setData(Qt::UserRole,data);
        ui->table_results->setHorizontalHeaderItem(headerNum, item);
    }

    for(int i = 0; i < ui->table_results->model()->rowCount(); i++)
    {
        qDebug() << ui->table_results->model()->headerData(i, Qt::Vertical, Qt::UserRole).toDouble();
        if (abs(n - ui->table_results->model()->headerData(i, Qt::Vertical, Qt::UserRole).toDouble()) < 1e-4){

            rowNum = i;
            break;
        }
    }

    if (rowNum == -1){
        ui->table_results->setRowCount(ui->table_results->model()->rowCount()+1);
        rowNum = ui->table_results->model()->rowCount()-1;

        QString name("N = " + QString::number(n));
        QTableWidgetItem* item = new QTableWidgetItem(name);
        QVariant data = n;
        item->setData(Qt::UserRole, data);
        ui->table_results->setVerticalHeaderItem(rowNum, item);
    }
    QTableWidgetItem* item = new QTableWidgetItem;
    //tabledrawingdata* data = new tabledrawingdata(t,n_y_max);
    item->setData(Qt::DisplayRole, QVariant::fromValue(tabledrawingdata(t,n_y_max)));
    ui->table_results->setItem(rowNum, headerNum, item);

    ui->table_results->verticalHeader()->setMinimumSectionSize(50);
    ui->table_results->horizontalHeader()->setMinimumSectionSize(175);

}

void ResultFrame::drawNy()
{
    QList<QTableWidgetItem*> items = ui->table_results->selectedItems();

    QItemSelectionModel *itemModel = ui->table_results->selectionModel();
    QModelIndexList indexList = itemModel->selectedRows();
    qDebug()<<Q_FUNC_INFO<<"IndexList Count"<<indexList.count();

    if (!indexList.isEmpty())
    {
        int row = indexList.at(0).row();
    }
}
