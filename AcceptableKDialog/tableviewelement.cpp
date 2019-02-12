#include "tableviewelement.h"
#include "ui_tableviewelement.h"

tableViewElement::tableViewElement(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::tableViewElement)
{
    ui->setupUi(this);
}

tableViewElement::~tableViewElement()
{
    delete ui;
}
