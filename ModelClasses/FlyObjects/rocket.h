#ifndef ROCKET_H
#define ROCKET_H

#include "la.h"
#include <vector>

using std::vector;

class Rocket: public QObject
{

  Q_OBJECT
public:
    Rocket(double x, double y, double z, double V, double n_xv,
           double n_yv, double teta, double psi, double gamma, LA* target, double K,
           const char* name = "Rocket");

    vector<double> TargetCoordinatesInSpeed;

private:
    double x,y,z;
    double V;
    double n_xv,n_yv;
    Angle teta,psi,gamma;

    double GetSigmaT();
    double CalcLambdaXZ();         //Book
    double CalcLambdaYX();
    vector<double> CalcV_XYprojection(vector<double> speed);
    double GetVT();             //Target speed
    double CalcXY_Angle();      //Book XY lambda

    bool checkForRoll();      //if |teta|>M_PI_2

    vector<double> toSpeedCoordinateSystem(vector<double> vec);
    vector<double> toTrajectoryCoordinateSystem(vector<double> vec);

    LA* target;//Target.
    double K;
    vector<double> TargetCoor;  //Coordinates of target in Speed System.
    double r;  //Distance to target XZ.
    double r_XY; //Distance to target XY.
    double r_explode; //Distance to target to explode

signals:
    void targetGetReached();

public slots:
    void update(double dt);
};

#endif // ROCKET_H
