#include "headeritem.h"
#include "ui_headeritem.h"

headerItem::headerItem(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::headerItem)
{
    ui->setupUi(this);
}

headerItem::~headerItem()
{
    delete ui;
}
