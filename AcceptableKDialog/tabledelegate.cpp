#include "tabledelegate.h"

#include "tableviewelement.h"

tableDelegate::tableDelegate(QObject *parent):
    QItemDelegate (parent)
{
}

QWidget *tableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    tableViewElement* elem = new tableViewElement(index, parent);
    return elem;
}

void tableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data(_t).toString();
    tableViewElement *fileDialog = static_cast<tableViewElement*> (editor);
    fileDialog->set_t(value);
}

void tableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    tableViewElement *fileDialog = static_cast<tableViewElement*> (editor);
    if (!fileDialog->getChoice().isEmpty())
        model->setData(index, fileDialog->getChoice());
}
