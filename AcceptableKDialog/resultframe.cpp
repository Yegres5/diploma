#include "resultframe.h"
#include "ui_resultframe.h"

#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>
#include <math.h>
#include <QString>

#include "tableviewelement.h"
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

//    QTableView* view = ui->table_results;
//    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    view->setDragEnabled(true);
//    view->setAcceptDrops(true);
//    view->setDropIndicatorShown(true);
//    tableDelegate *delegate = new tableDelegate (this);
    ui->table_results->setItemDelegate(new tableDelegate);

    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->table_results->model());
    model->setRowCount(2);
    model->setColumnCount(2);

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setData(_t,10);
    ui->table_results->setItem(0, 0, item);

    ui->table_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_results->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //ui->table_results->selectedItems();
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

//    tableViewElement* elem = new tableViewElement();
//    ui->table_results->setCellWidget(rowNum, headerNum, elem);
//    QTableWidgetItem item;

//    ui->table_results->verticalHeader()->setMinimumSectionSize(elem->size().height());
//    ui->table_results->horizontalHeader()->setMinimumSectionSize(elem->size().width());

    qDebug();
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
