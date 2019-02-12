#include "resultframe.h"
#include "ui_resultframe.h"

#include <QLabel>
#include <QPushButton>

#include "tableviewelement.h"

ResultFrame::ResultFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ResultFrame)
{
    ui->setupUi(this);
    ui->table_results->setRowCount(50);
    ui->table_results->setColumnCount(50);
    ui->table_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_results->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableViewElement* elem = nullptr;
    for (int i(0);i!=50;i++){
        for (int j(0);j!=50;j++){
            elem = new tableViewElement();
            ui->table_results->setCellWidget(i,j,elem);
        }
    }

    ui->table_results->horizontalHeader()->setMinimumSectionSize(elem->size().width());
    ui->table_results->verticalHeader()->setMinimumSectionSize(elem->size().height());
    ui->table_results->setMinimumSize(640,640);

    ui->table_results->selectedItems();
}

ResultFrame::~ResultFrame()
{
    delete ui;
}
