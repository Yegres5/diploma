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
    Model(QMap<QString, QVariant>* iniParam);
public slots:
    void StartModeling();
    void StartModelingFor(double K, QList<double> angles, double dt);
    void writeCoordToCSV(QMap<QString,  QVariant>* coord);
    void clearCSVFiles();
signals:
    void startSimulate(double k);
    void sendData(QMap<QString,double>* tempMap, QVector<double>* n_y);
};



#endif // MODEL_H
