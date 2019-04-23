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
    void startSimulationFor(QList<double> K, QList<double> angles, double dt);
    void hideDialog();
    void showDialog();

public slots:
    void pasteData(QMap<QString, double>* modelingParametrs, QMap<QString,QList<double>>* graphs);

private slots:
    void drawNy();
    void drawNz();
    void drawNy_sum();
    void draw3Dtrajectory();
private:
    void drawGraphForKey(QString key);
};



#endif // RESULTFRAME_H
