#ifndef LAINITIALFRAME_H
#define LAINITIALFRAME_H

#include <QFrame>

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


};

#endif // LAINITIALFRAME_H
