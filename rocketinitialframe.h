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
};

#endif // ROCKETINITIALFRAME_H
