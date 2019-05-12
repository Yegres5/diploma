#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <QWidget>

namespace Ui {
class dbConnector;
}

class dbConnector : public QWidget
{
    Q_OBJECT

public:
    explicit dbConnector(QWidget *parent = nullptr);
    ~dbConnector();

private slots:
    void on_pushButton_check_clicked();

    void on_pushButton_connect_clicked();

signals:
    void send_dbParameters(QList<QVariant>* data);

private:
    Ui::dbConnector *ui;
};

#endif // DBCONNECTOR_H
