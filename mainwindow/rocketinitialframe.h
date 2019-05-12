#ifndef ROCKETINITIALFRAME_H
#define ROCKETINITIALFRAME_H

#include <QFrame>

namespace Ui {
    class rocketInitialFrame;
}

class rocketInitialFrame: public QFrame
{
    Q_OBJECT
private:
    Ui::rocketInitialFrame* ui;
    QList<QVariant>* dbIniParameters;

public:
    explicit rocketInitialFrame(QWidget* parent = nullptr);
    ~rocketInitialFrame();

    void loadInitialParametrs(QMap<QString, double> parametrs);

    QMap<QString, double> getInitialParametrs();
    QMap<QString, double>* rocketData;

private slots:
    void on_pushButton_clicked();
    void setRocketData(QMap<QString, double>* data);
    void setDBParameters(QList<QVariant>* data);

signals:
    void setModelingButtonOn();
};

#endif // ROCKETINITIALFRAME_H
