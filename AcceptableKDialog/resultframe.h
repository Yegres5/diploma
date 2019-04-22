#ifndef RESULTFRAME_H
#define RESULTFRAME_H

#include <QFrame>
#include <QVector>
#include <QMap>
#include <QMetaType>

namespace Ui {
class ResultFrame;
}

class ResultFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ResultFrame(QWidget *parent = nullptr);
    ~ResultFrame();

    Ui::ResultFrame *ui;

signals:
    void startSimulation();
    void startSimulationFor(double K, QList<double> angles, double dt);
    void hideDialog();
    void showDialog();

public slots:
    void pasteData(QMap<QString, double>* modelingParametrs, QVector<double> *n_y);

private slots:
    void drawNy();
    void draw3Dtrajectory();
};



#endif // RESULTFRAME_H
