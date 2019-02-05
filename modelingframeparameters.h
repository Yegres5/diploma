#ifndef MODELINGFRAMEPARAMETERS_H
#define MODELINGFRAMEPARAMETERS_H

#include <QFrame>

namespace Ui {
class modelingFrameParameters;
}

class modelingFrameParameters : public QFrame
{
    Q_OBJECT

public:
    explicit modelingFrameParameters(QWidget *parent = nullptr);
    ~modelingFrameParameters();

private:
    Ui::modelingFrameParameters *ui;
};

#endif // MODELINGFRAMEPARAMENTS_H
