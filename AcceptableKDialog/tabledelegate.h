#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H
#include <QItemDelegate>


class tableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    tableDelegate(QObject* parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,               
                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;
};

#endif // TABLEDELEGATE_H
