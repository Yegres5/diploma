#ifndef ROCKET_H
#define ROCKET_H

#include "la.h"
#include <QVector>

class Rocket: public QObject
{

  Q_OBJECT
public:
    Rocket(double x, double y, double z, double V, double n_xv,
           double n_yv, double teta, double psi, double gamma, LA* target, double K,
           const char* name = "Rocket");

    QVector<double> TargetCoordinatesInSpeed;
    double n_y_max;
    double getNy();
    double getDistanceToTarget() {return distance_to_target;}

    double n_pitch;
    double n_roll;
private:
    double x,y,z;
    double V;
    double n_xv,n_yv;
    Angle teta,psi,gamma;

//    double GetSigmaT();
    double CalcLambdaXZ();         //Book
    double CalcLambdaYX();
//    QVector<double> CalcV_XYprojection(QVector<double> speed);
//    double GetVT();             //Target speed
//    double CalcXY_Angle();      //Book XY lambda

//    bool checkForRoll();

    QVector<double> toSpeedCoordinateSystem(QVector<double> vec);
    QVector<double> toTrajectoryCoordinateSystem(QVector<double> vec);

    LA* target;//Target.
    double K;
    QVector<double> TargetCoor;  //Coordinates of target in Speed System.
    double distance_to_target;
    double r;  //Distance to target XZ.
    double r_XY; //Distance to target XY.
    double r_explode; //Distance to target to explode
signals:
    void targetGetReached();

public slots:
    void update(double dt);
};

#endif // ROCKET_H
