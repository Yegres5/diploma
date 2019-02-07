#ifndef MODELINGFRAMEPARAMETERS_H
#define MODELINGFRAMEPARAMETERS_H

#include <QFrame>

namespace Ui {
class modelingFrameParameters;
}

class modelingFrameParameters : public QFrame
{
    Q_OBJECT
private:
    Ui::modelingFrameParameters *ui;

public:
    explicit modelingFrameParameters(QWidget *parent = nullptr);
    ~modelingFrameParameters();

    QMap<QString, double> getInitialParametrs();
    void loadInitialParametrs(QMap<QString, double> parametrs);
};

#endif // MODELINGFRAMEPARAMENTS_H
