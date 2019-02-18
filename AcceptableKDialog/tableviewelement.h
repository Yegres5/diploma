#ifndef TABLEVIEWELEMENT_H
#define TABLEVIEWELEMENT_H

#include <QFrame>

#include "resultframe.h"

#define _t Qt::UserRole
#define _n_y_max Qt::UserRole+1

namespace Ui {
class tableViewElement;
}

class tableViewElement : public QWidget
{
    Q_OBJECT

public:
    explicit tableViewElement(const QModelIndex &index, QWidget *parent = nullptr);
    void set_t(QString t);
    QString getChoice();
    ~tableViewElement();

private:
    Ui::tableViewElement *ui;

    const QModelIndex& index;
    QVector<double>* n_y;
    double dt;

    friend ResultFrame;
};

#endif // TABLEVIEWELEMENT_H
#include <QItemDelegate>
