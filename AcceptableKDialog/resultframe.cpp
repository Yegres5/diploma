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
#include <algorithm>
#include <limits>

#include "tabledrawingdata.h"
#include "headeritem.h"
#include "tabledelegate.h"

#define Role_Map Qt::UserRole
#define Role_graph Qt::UserRole+1
#define Role_error Qt::UserRole+2

ResultFrame::ResultFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ResultFrame)
{
    ui->setupUi(this);
    connect(ui->button_startSimulate,SIGNAL(clicked()),
            this, SIGNAL(startSimulation()));
    connect(ui->button_draw_ny,SIGNAL(clicked()),
            this, SLOT(drawNy()));
    connect(ui->button_draw_ny_sum,SIGNAL(clicked()),
            this, SLOT(drawNy_sum()));
    connect(ui->button_draw_nz,SIGNAL(clicked()),
            this, SLOT(drawNz()));
    connect(ui->button_optimal_plan, SIGNAL(clicked()),
            this, SLOT(findOptimalPlan()));

    connect(ui->button_start3dModeling,SIGNAL(clicked()),
            this, SLOT(draw3Dtrajectory()));

    ui->table_modeling_results->setColumnCount(1);

    ui->table_modeling_results->setHorizontalHeaderLabels(QStringList({tr("Parameter"), tr("Value")}));
    ui->table_modeling_results->setRowCount(4);
    ui->table_modeling_results->setVerticalHeaderLabels(QStringList({tr("V_end"), tr("Min distance to target"), tr("Max overload"), tr("Max angle on sight")}));

    for (int i(0); i < ui->table_modeling_results->rowCount(); i++) {
        for (int j(0); j < ui->table_modeling_results->columnCount(); j++){
            ui->table_modeling_results->setItem(i,j,new QTableWidgetItem());
        }
    }

    ui->table_modeling_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_modeling_results->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void ResultFrame::setMaxPercent(int percent)
{
    ui->progressBar->setMaximum(percent);
}

void ResultFrame::pasteData(QMap<QString, double>* modelingParametrs, QMap<QString,QList<double>>* graphs)
{
    int headerNum(-1),rowNum(-1);
    ui->progressBar->setValue(ui->progressBar->value() + 1);

    //finding K
    for(int i = 0; i < ui->table_results->model()->columnCount(); i++)
    {

        QList<double> kList(ui->table_results->model()->headerData(i, Qt::Horizontal,Qt::UserRole).value<QList<double>>());

        if ((abs(kList.first() - *modelingParametrs->find("Rock Ky")) < 1e-4) &&
                (abs(kList.last() - *modelingParametrs->find("Rock Kz")) < 1e-4)) {
            headerNum = i;
            break;
        }
    }

    //if K not found
    if (headerNum == -1){

        ui->table_results->setColumnCount(ui->table_results->model()->columnCount()+1);
        headerNum = ui->table_results->model()->columnCount()-1;


        QList<double> kList;
        kList.push_back(*modelingParametrs->find("Rock Ky"));
        kList.push_back(*modelingParametrs->find("Rock Kz"));

        QVariant data;
        data.setValue<QList<double>>(kList);

        QString name(tr("Ky = ") + QString::number(kList.first()) + "\n" +
                     tr("Kz = ") + QString::number(kList.last()));
        QTableWidgetItem* item = new QTableWidgetItem(name);

        item->setData(Qt::UserRole,data);
        ui->table_results->setHorizontalHeaderItem(headerNum, item);
    }

    //finding (delta lambda)

    for(int i = 0; i < ui->table_results->model()->rowCount(); i++)
    {
        QList<double> angleList(ui->table_results->model()->headerData(i, Qt::Vertical, Qt::UserRole).value<QList<double>>());
        angleList[0] = angleList[0]/180*M_PI;
        angleList[1] = angleList[1]/180*M_PI;
        if ((abs(*modelingParametrs->find("LA delta") - angleList.first()) < 1e-4)
                && (abs(*modelingParametrs->find("LA lambda") - angleList.last()) < 1e-4)){
            rowNum = i;
            break;
        }
    }

    //if (delta lambda) not found
    if (rowNum == -1){

        ui->table_results->setRowCount(ui->table_results->model()->rowCount()+1);
        rowNum = ui->table_results->model()->rowCount()-1;

        QList<double> angles;
        angles.push_back(*modelingParametrs->find("LA delta")/M_PI*180);
        angles.push_back(*modelingParametrs->find("LA lambda")/M_PI*180);

        QString name(tr("Delta = ") + QString::number(angles.first()) + "\n"
                     + tr("Lambda = ") + QString::number(angles.last()));

        QTableWidgetItem* item = new QTableWidgetItem(name);

        QVariant data;
        data.setValue<QList<double>>(angles);
        item->setData(Qt::UserRole, data);
        ui->table_results->setVerticalHeaderItem(rowNum, item);
    }

    //paste element in cell
    QTableWidgetItem* item = new QTableWidgetItem;

    QMap<QString, double> dataMap;
    dataMap.insert("ky", *modelingParametrs->find("Rock Ky"));
    dataMap.insert("kz", *modelingParametrs->find("Rock Kz"));
    dataMap.insert("t", *modelingParametrs->find("Simulator t"));
    dataMap.insert("dt", *modelingParametrs->find("Simulator dt"));
    dataMap.insert("n_y_max", *modelingParametrs->find("Simulator n_y_max"));

    dataMap.insert("V_end", *modelingParametrs->find("V_end"));
    dataMap.insert("Min distance to target", *modelingParametrs->find("Min distance to target"));
    dataMap.insert("Max overload", *modelingParametrs->find("Max overload"));
    dataMap.insert("Max angle of sight", *modelingParametrs->find("Max angle of sight"));



    item->setData(Role_graph, QVariant::fromValue(*graphs));


    QString errorMessage;
    switch (int(*modelingParametrs->find("Error message"))) {
        case 0:
            break;
        case 1:
            errorMessage = tr("Out of time");
        break;
        case 2:
            errorMessage = tr("Out of angle");
        break;
    }
    item->setData(Qt::DisplayRole, QVariant::fromValue(tabledrawingdata(*modelingParametrs->find("Simulator t"),
                                                                        *modelingParametrs->find("Simulator n_y_max"),
                                                                        errorMessage)));
    dataMap.insert("Error message", *modelingParametrs->find("Error message"));
    item->setData(Role_Map, QVariant::fromValue(dataMap));


    ui->table_results->setItem(rowNum, headerNum, item);
    ui->table_results->verticalHeader()->setMinimumSectionSize(50);
    ui->table_results->horizontalHeader()->setMinimumSectionSize(175);
}

void ResultFrame::drawNy()
{
    drawGraphForKey("N_y");
}

void ResultFrame::drawNz()
{
    drawGraphForKey("N_z");
}

void ResultFrame::drawNy_sum()
{
    drawGraphForKey("Ny_sum");
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
        const QList<double> k = ui->table_results->horizontalHeaderItem(item->column())->data(Qt::UserRole).value<QList<double>>();
        const QList<double> angles(ui->table_results->verticalHeaderItem(item->row())->data(Qt::UserRole).value<QList<double>>());
        emit startSimulationFor(k,angles,ui->Edit_modelingTime->text().toDouble());
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
    QStringList arguments { obj.value("pythonScriptPath").toString() + "3Ddraw_animate.py" };

    QProcess p;
    p.start(obj.value("pythonInterpriterPath").toString(), arguments);
    p.waitForFinished(-1);
}

void ResultFrame::findOptimalPlan()
{
    QVector<QVector<double>> timeVec;
    for(int i = 0; i < ui->table_results->model()->rowCount(); i++){
        QVector<double> timeLine;
        for(int j = 0; j < ui->table_results->model()->columnCount(); j++){
            QMap<QString,double> data = qvariant_cast<QMap<QString,double>>(ui->table_results->item(i,j)->data(Role_Map));
            if ((*data.find("Error message") < 1e-7)){
                timeLine.push_back(*data.find("t"));
            }else{
                timeLine.push_back(std::numeric_limits<double>::max());
            }
            qDebug();
        }
        timeVec.push_back(timeLine);
    }

    QVector<double> MaxVec(timeVec.first().size());
    for (auto& i : timeVec) {
        for (int j(0); j < i.size(); j++) {
            if  (MaxVec[j] < i[j]){
                MaxVec[j] = i[j];
            }
        }
    }

    int numberK(0);
    double MinValue = std::numeric_limits<double>::max();
    for (int i(0); i < MaxVec.size(); i++) {
        if (MaxVec[i] < MinValue){
            numberK = i;
            MinValue = MaxVec[i];
        }
    }

    QList<double> kList(ui->table_results->model()->headerData(numberK, Qt::Horizontal,Qt::UserRole).value<QList<double>>());
    QMessageBox msgBox;
    msgBox.setText(tr("Ky = ") + QString::number(kList.first()) + tr(" Kz = ") + QString::number(kList.last()) + tr(" Time = ") + QString::number(MinValue));
    msgBox.exec();
}

void ResultFrame::drawGraphForKey(QString key)
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
        QMap<QString,QList<double>> graphs = qvariant_cast<QMap<QString,QList<double>>>(item->data(Role_graph));
        QList<double> n_y = *graphs.find(key);
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

void ResultFrame::on_table_results_cellClicked(int row, int column)
{
    QTableWidgetItem* item = ui->table_results->item(row,column);
    QMap<QString,double> map = qvariant_cast<QMap<QString,double>>(item->data(Role_Map));

    ui->table_modeling_results->item(0,0)->setText(QString::number(*map.find("V_end")));
    ui->table_modeling_results->item(1,0)->setText(QString::number(*map.find("Min distance to target")));
    ui->table_modeling_results->item(2,0)->setText(QString::number(*map.find("Max overload")));
    ui->table_modeling_results->item(3,0)->setText(QString::number(*map.find("Max angle of sight")/M_PI*180));
}
