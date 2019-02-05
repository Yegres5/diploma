#include "rocketinitialframe.h"
#include "ui_rocketinitialframe.h"

rocketInitialFrame::rocketInitialFrame(QWidget* parent):
    QFrame (parent),
    ui(new Ui::rocketInitialFrame)
{
    ui->setupUi(this);
}

rocketInitialFrame::~rocketInitialFrame()
{
    delete ui;
}
