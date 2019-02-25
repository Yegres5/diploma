#include "resultframe.h"
#include "ui_resultframe.h"

#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>
#include <math.h>
#include <QString>
#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>
#include <QtCharts>
#include <QList>

#include "tabledrawingdata.h"
#include "headeritem.h"
#include "tabledelegate.h"

#define Role_Map Qt::UserRole
#define Role_Ny Qt::UserRole+1

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
    item->setData(Qt::DisplayRole, QVariant::fromValue(tabledrawingdata(t,n_y_max)));
    QMap<QString, double> dataMap;
    dataMap.insert("k", k);
    dataMap.insert("t", t);
    dataMap.insert("dt" ,dt);
    dataMap.insert("n", n);
    dataMap.insert("n_y_max", n_y_max);
    item->setData(Role_Map, QVariant::fromValue(dataMap));
    item->setData(Role_Ny, QVariant::fromValue(n_y->toList()));
    ui->table_results->setItem(rowNum, headerNum, item);

    ui->table_results->verticalHeader()->setMinimumSectionSize(50);
    ui->table_results->horizontalHeader()->setMinimumSectionSize(175);

}

void ResultFrame::drawNy()
{
    QList<QTableWidgetItem*> items = ui->table_results->selectedItems();

    qDebug()<<Q_FUNC_INFO<<"Items Count"<<items.count();

    if (!items.count())
    {
        qDebug() << "Return";
        return;
    }

    QLineSeries *series = new QLineSeries();
    for (QList<QTableWidgetItem*>::iterator it(items.begin()); it != items.end(); it++){
        QTableWidgetItem* item = *it;

        QList<double> n_y = qvariant_cast<QList<double>>(item->data(Role_Ny));
        double dt = qvariant_cast<QMap<QString,double>>(item->data(Role_Map)).find("dt").value();
        double t(0);

        for (QList<double>::iterator ListIter(n_y.begin()); ListIter != n_y.end(); ListIter++) {
            series->append(t, *ListIter);
            t += dt;
        }
        QChart *chart= new QChart();
        chart->addSeries(series);
        chart->setTitle("График зависимости перегрузки ЛА от времени");

        QValueAxis *axisX = new QValueAxis;
        axisX->setTickCount(10);
        axisX->setTitleText("dZ");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QChartView* v = new QChartView;
        v->setChart(chart);
        QDialog* dia = new QDialog();
        QVBoxLayout* layout = new QVBoxLayout(dia);
        dia->setLayout(layout);
        layout->addWidget(v);

        dia->exec();

    }
}
