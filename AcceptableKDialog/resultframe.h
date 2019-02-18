#ifndef RESULTFRAME_H
#define RESULTFRAME_H

#include <QFrame>
#include <QVector>

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
public slots:
    void pasteData(double k, double n, double t, double dt, double n_y_max, QVector<double> *n_y);
private slots:
    void drawNy();
private:
    Ui::ResultFrame *ui;
};

#endif // RESULTFRAME_H
