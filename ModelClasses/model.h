#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject
{
    Q_OBJECT

private:
    double n0, n1, dn;
    double k0,k1,dk;
    QMap<QString, QVariant>* params;
public:
    Model(QMap<QString, QVariant>* iniParam);

public slots:
    void StartModeling();

signals:
    void startSimulate(double k, double n);
    void sendData(double k, double n, double t, double dt, double n_y_max, QVector<double>* n_y);

};

#endif // MODEL_H
