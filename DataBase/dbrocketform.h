#ifndef DBROCKETFORM_H
#define DBROCKETFORM_H

#include <QFrame>
#include "databaseparser.h"

#include <QGraphicsScene>
#include <QListWidgetItem>

namespace Ui {
class DBRocketForm;
}

class DBRocketForm : public QFrame
{
    Q_OBJECT

public:
    explicit DBRocketForm(QWidget *parent = nullptr);
    ~DBRocketForm() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void on_listWidget_Rockets_itemPressed(QListWidgetItem *item);

    void on_button_Change_Selected_clicked();

    void on_graphicsView_Image_customContextMenuRequested(const QPoint &pos);

private:
    Ui::DBRocketForm *ui;
    dataBaseParser DB_Parser;
};

#endif // DBROCKETFORM_H
