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
    connect(ui->button_start3dModeling,SIGNAL(clicked()),
            this, SLOT(draw3Dtrajectory()));

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

    if (!items.count())
    {
        qDebug() << "Return";
        return;
    }

    QChart *chart= new QChart();
    chart->setTitle(tr("Graph of LA overload versus time"));
    QLineSeries* series = nullptr;

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText(tr("Time"));
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText(tr("Overload"));
    chart->addAxis(axisY, Qt::AlignLeft);

    {
    int r(255),g(0),b(0);
    for (QList<QTableWidgetItem*>::iterator it(items.begin()); it != items.end(); it++){
        QTableWidgetItem* item = *it;
        series = new QLineSeries(chart);

        QBrush brush(QColor::fromRgb(r,g,b));
        series->setPen(QPen(brush, 4));
        QList<double> n_y = qvariant_cast<QList<double>>(item->data(Role_Ny));
        double dt = qvariant_cast<QMap<QString,double>>(item->data(Role_Map)).find("dt").value();
        double t(0);

        for (QList<double>::iterator ListIter(n_y.begin()); ListIter != n_y.end(); ListIter++) {
            series->append(t, *ListIter);
            t += dt;
        }

        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        if (axisX->max() < t){
            axisX->setMax(t);
        }

        if (axisY->max() < qvariant_cast<QMap<QString,double>>(item->data(Role_Map)).find("n_y_max").value()){
            axisY->setMax(qvariant_cast<QMap<QString,double>>(item->data(Role_Map)).find("n_y_max").value());
        }

        double minimum_ny = *std::min_element(n_y.begin(), n_y.end());

        if (axisY->min() < minimum_ny + 0.5){
            axisY->setMin(minimum_ny - 0.5);
        }

        if (g+50 < 255){
            g += 70;
        }else{
            g = 70;
            b += 80;
        }
    }
    }
    axisX->setTickCount(15);
    axisX->setMin(0);

    QChartView* v = new QChartView;
    v->setChart(chart);
    QDialog* dia = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dia);
    dia->setLayout(layout);
    layout->addWidget(v);

    dia->exec();
}

void ResultFrame::draw3Dtrajectory()
{

    QList<QTableWidgetItem*> items = ui->table_results->selectedItems();

    if (!items.count())
    {
        qDebug() << "Return";
        QMessageBox msgBox;
        msgBox.setText(tr("Choose element."));
        msgBox.exec();
        return;
    }

    switch (items.count()){

    case 0:{
        QMessageBox msgBox;
        msgBox.setText(tr("Choose element."));
        msgBox.exec();
        break;
        }
    case 1: {
        QTableWidgetItem* item = items.takeFirst();
        const double k = ui->table_results->horizontalHeaderItem(item->column())->data(Qt::UserRole).toDouble();
        const double n_y = ui->table_results->verticalHeaderItem(item->row())->data(Qt::UserRole).toDouble();

        emit startSimulationFor(k,n_y,ui->Edit_modelingTime->text().toDouble());


        break;
        }
    default:{
        QMessageBox msgBox;
        msgBox.setText(tr("Choose only one element."));
        msgBox.exec();
        return;
        }
    }

    QFile jsonFile(":/JSON/paths.json");
    jsonFile.open(QFile::ReadOnly);

    QJsonDocument doc (QJsonDocument().fromJson(jsonFile.readAll()));
    QJsonObject obj (doc.object());
    QStringList arguments { obj.value("pythonScriptPath").toString() + "3Ddraw.py" };

    QProcess p;
    p.start(obj.value("pythonInterpriterPath").toString(), arguments);
    p.waitForFinished(-1);
}
