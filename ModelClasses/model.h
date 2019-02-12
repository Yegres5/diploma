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

};

#endif // MODEL_H
