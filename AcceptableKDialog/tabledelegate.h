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

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const;

//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;
};

#endif // TABLEDELEGATE_H
