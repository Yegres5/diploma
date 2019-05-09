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

public:
    explicit rocketInitialFrame(QWidget* parent = nullptr);
    ~rocketInitialFrame();

    QMap<QString, double> getInitialParametrs();
    void loadInitialParametrs(QMap<QString, double> parametrs);
private slots:
    void on_pushButton_clicked();
};

#endif // ROCKETINITIALFRAME_H
