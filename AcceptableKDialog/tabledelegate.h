#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H
#include <QStyledItemDelegate>


class tableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    tableDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // TABLEDELEGATE_H
