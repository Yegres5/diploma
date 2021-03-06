#ifndef ROCKET_H
#define ROCKET_H

#include "la.h"
#include <QVector>
#include <QtMath>

class Rocket: public QObject
{

  Q_OBJECT
public:
    Rocket(double x, double y, double z, double V, double teta, double psi, LA* target, double Ky, double Kz, double max_distance_to_target, double maxAngle, double startHight,
           double l, double d, double Ae, double Sm, double Ln, double Nmax,
           const char* name = "Rocket");

    double getNy()      { return n_yv; }
    double getN_pitch() { return n_pitch; }
    double getN_roll()  { return n_roll; }
    double getDistanceToTarget() { return distance_to_target; }
    double getX()       { return x; }
    double getY()       { return y; }
    double getZ()       { return z; }
    double getV()       { return V; }
    double getAngleOfSight() { return angleOfSight; }

private:
    double x,y,z;                   //Self coordinates.
    double V;                       //Absolute speed value.
    double n_xv,n_yv;               //X Y overload.
    Angle teta,psi,gamma;           //Euler angles.
    LA* target;                     //Target.
    double Ky,Kz;                   //Coefficient of proportional navigation.
    double n_y_max;                 //Maximum Ny overload.
    QVector<double> TargetCoor;     //Coordinates of target in Speed System.
    double distance_to_target;      //Current distance to target.
    double max_distance_to_target;
    double r;                       //Distance to target XZ.
    double r_XY;                    //Distance to target XY.
    double n_pitch;                 //Normal overload.
    double n_roll;                  //Lateral overload.
    double angleOfSight, max_angleLineOfSight;

    double startHight;
    double l;
    double d;
    double Ae;
    double Sm;
    double Ln;

    double p();    //Density on hight
    double Mc();   //Mach number on hight

    double q(){ return 0.5*p()*pow(V,2); } //Dynamic pressure
    double M(){ return V/Mc(); }   //Current Mach

    QVector<double> TargetSpeed;

    void GravityCompensation();         //Adding overload to n_y, n_x, and changing gamma.
    void CalculateTargetPosition();     //Calculating target position in SpeedCoordinateSystem.
    void CalculateTargetSpeed();        //Calculating target speed vector.
    void CalculateNyPN();
    void CalculateNzPN();
    void SummarizeAllOverload();
    void CheckMaxNy();
    void EquationsOfMotion(double dt);
    void CheckTargetGetReached();
    void CalculatingDragForce();
    void CheckMaxAngle();

    QVector<double> toSpeedCoordinateSystem(QVector<double> vec);
    QVector<double> toTrajectoryCoordinateSystem(QVector<double> vec);
signals:
    void targetGetReached(int code);

public slots:
    void update(double dt);
};

#endif // ROCKET_H
