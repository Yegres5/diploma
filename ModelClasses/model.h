#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QMetaType>
#include <QMap>
class Model : public QObject
{
    Q_OBJECT

private:
    QMap<QString, QVariant>* params;
    QMap<QString, QVariant> staticParams;
public:
    Model(QMap<QString, QVariant>* iniParam, QObject *parent = nullptr);
public slots:
    void StartModeling();
    void StartModelingFor(QList<double> K, QList<double> angles, double dt);
    void writeCoordToCSV(QMap<QString,  QVariant>* coord);
    void clearCSVFiles();
signals:
    void startSimulate(double Ky, double Kz);
    void sendData(QMap<QString,double>* tempMap, QMap<QString, QList<double>>* n_y);
};



#endif // MODEL_H
