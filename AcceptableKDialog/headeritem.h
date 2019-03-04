#ifndef HEADERITEM_H
#define HEADERITEM_H

#include <QFrame>

namespace Ui {
class headerItem;
}

class headerItem : public QFrame
{
    Q_OBJECT

public:
    explicit headerItem(QWidget *parent = nullptr);
    ~headerItem();

private:
    Ui::headerItem *ui;
};

#endif // HEADERITEM_H
