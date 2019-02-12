#ifndef RESULTFRAME_H
#define RESULTFRAME_H

#include <QFrame>

namespace Ui {
class ResultFrame;
}

class ResultFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ResultFrame(QWidget *parent = nullptr);
    ~ResultFrame();
signals:
    void startSimulation();
private:
    Ui::ResultFrame *ui;
};

#endif // RESULTFRAME_H
