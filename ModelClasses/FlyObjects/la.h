#ifndef LA_H
#define LA_H

#include <QObject>
#include "UsefulClassesForLA/angle.h"

#define _g 9.81

class LA : public QObject
{
    Q_OBJECT

protected:
    double x,y,z;
    double V;
    double n_xv,n_yv;
    double n_roll;
    Angle teta,psi,gamma;
    double n_manouver,tManouver;    //Manouver overload, manouver initial time.
    double t;                       //Current time(for manouver).
    double deltaAngle;
    bool startSnake;
    bool manouver_flag;
    double angleForSnake;
    bool increasedAngleForShake;

public:
    LA(double x,double y,double z,double V,double n_xv,
       double n_yv,double teta,double psi,double gamma,
       double n_manouver, double tManouver, double deltaAngle, double angleForSnake,
       const char* name = "LA");

    explicit LA(QObject *parent = nullptr);
    LA();

    double getX()       { return x; }
    double getY()       { return y; }
    double getZ()       { return z; }
    double GetV()       { return V; }
    double GetNy()      { return n_yv; }
    double GetNx()      { return n_xv; }
    Angle getTeta()     { return teta; }
    Angle getPsi()      { return psi; }
    Angle getGamma()    { return gamma; }

public slots:
   virtual void update(double dt);

private:
    bool makeManouverToAngle(double angle);

};

#endif // LA_H
