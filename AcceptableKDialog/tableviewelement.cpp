#include "tableviewelement.h"
#include "ui_tableviewelement.h"



tableViewElement::tableViewElement(const QModelIndex &index, QWidget *parent):
    QWidget (parent),
    index(index)
{
    ui->value_t->setText(index.data(_t).toString());
}

void tableViewElement::set_t(QString t)
{
    ui->value_t->setText(t);
}

QString tableViewElement::getChoice()
{
    return ui->value_t->text();
}

tableViewElement::~tableViewElement()
{
    delete ui;
}
