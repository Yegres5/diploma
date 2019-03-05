#ifndef LAINITIALFRAME_H
#define LAINITIALFRAME_H

#include <QFrame>
#include <QMap>

namespace Ui {
    class laInitialFrame;
}

class laInitialFrame: public QFrame
{
    Q_OBJECT
private:
    Ui::laInitialFrame* ui;

public:
    laInitialFrame(QWidget* parent = nullptr);
    ~laInitialFrame();

    QMap<QString, double> getInitialParametrs();
    void loadInitialParametrs(QMap<QString, double> parametrs);
};

#endif // LAINITIALFRAME_H
