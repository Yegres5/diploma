#include "lainitialframe.h"
#include "ui_lainitialframe.h"

laInitialFrame::laInitialFrame(QWidget *parent):
    QFrame(parent),
    ui(new Ui::laInitialFrame)
{
    ui->setupUi(this);
}

laInitialFrame::~laInitialFrame()
{
    delete ui;
}
