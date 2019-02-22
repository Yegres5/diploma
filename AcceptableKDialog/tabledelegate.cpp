#include "tabledelegate.h"

#include <QPainter>

#include "tabledrawingdata.h"

tableDelegate::tableDelegate(QObject *parent):
    QStyledItemDelegate (parent)
{
}

void tableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<tabledrawingdata>()) {
        tabledrawingdata tbldata = qvariant_cast<tabledrawingdata>(index.data());

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        tbldata.paint(painter, option.rect, option.palette);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize tableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<tabledrawingdata>()) {
        tabledrawingdata tbldata = qvariant_cast<tabledrawingdata>(index.data());
        return tbldata.sizeHint();
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

//QWidget *tableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    tableViewElement* elem = new tableViewElement(index, parent);
//    return elem;
//}

//void tableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    QString value = index.data(_t).toString();
//    tableViewElement *fileDialog = static_cast<tableViewElement*> (editor);
//    fileDialog->set_t(value);
//}

//void tableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//    tableViewElement *fileDialog = static_cast<tableViewElement*> (editor);
//    if (!fileDialog->getChoice().isEmpty())
//        model->setData(index, fileDialog->getChoice());
//}
