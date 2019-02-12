#ifndef TABLEVIEWELEMENT_H
#define TABLEVIEWELEMENT_H

#include <QFrame>

#include "resultframe.h"

namespace Ui {
class tableViewElement;
}

class tableViewElement : public QFrame
{
    Q_OBJECT

public:
    explicit tableViewElement(QWidget *parent = nullptr);
    ~tableViewElement();

private:
    Ui::tableViewElement *ui;

    friend ResultFrame;
};

#endif // TABLEVIEWELEMENT_H
