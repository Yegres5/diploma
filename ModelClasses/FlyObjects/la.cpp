#include "la.h"
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include <QVector>

#define isDoubleEqualToZero(x) ( fabs(x) < 0.1e-5)

LA::LA()
{}

LA::LA(double x, double y, double z, double V, double n_xv,
       double n_yv, double teta, double psi, double gamma, double n_manouver, double n_t0, double n_dt, const char *name):
        x(x),y(y),z(z),V(V),n_xv(n_xv),n_yv(n_yv),teta(teta/180*M_PI),psi(psi/180*M_PI),gamma(gamma/180*M_PI),n_manouver(n_manouver),n_t0(n_t0),n_dt(n_dt),t(0)
{
    setObjectName(name);
}

void LA::update(double dt)
{
    //qDebug("%.3f,%.3f,%.3f",x,y,z);

    QVector<double> grav = {0,1,0};
    {
    double psi = 0;
    double teta  = -this->teta.getValue();
    double gamma = -this->gamma.getValue();

    QVector<QVector<double>> arr{
                        {cos(teta)*cos(psi),                                    sin(teta),              -cos(teta)*sin(psi)},
                        {-cos(gamma)*sin(teta)*cos(psi)+sin(gamma)*sin(psi), cos(gamma)*cos(teta),  cos(gamma)*sin(teta)*sin(psi)+sin(gamma)*cos(psi)},
                        {sin(gamma)*sin(teta)*cos(psi)+cos(gamma)*sin(psi),  -sin(gamma)*cos(teta), -sin(psi)*sin(teta)*sin(gamma)+cos(psi)*cos(gamma)}};

    QVector<double> temp(3);

    for(int i = 0; i < arr.at(0).size(); i++){
        for(int j = 0; j < grav.size(); j++){
            temp[i] += grav.at(j)*arr.at(j).at(i);
        }
    }

    grav = temp;
    }

    gamma += isDoubleEqualToZero(grav[1]) ? 0 : atan(grav[2]/
                                            sqrt(pow(grav[0],2)+pow(grav[1],2)));

    if (isDoubleEqualToZero(grav[0])){
        n_yv = sqrt(pow(grav[1],2)+pow(grav[2],2));
        n_xv = 0;
    }else{
        if (isDoubleEqualToZero(grav[1])){
            n_yv = fabs(grav[2]);
            n_xv = grav[0];
        }else{
            if (isDoubleEqualToZero(grav[2])){
                n_yv = grav[1];
                n_xv = grav[0];
            }else{
                  n_yv = sqrt(pow(grav[2],2)+pow(grav[1],2));
                  n_xv = grav[0];
            }
        }
    }
    n_yv += 0;
    double n_roll = 0;
    t += dt;

    if (t > n_t0){
        n_roll = n_manouver;
    }
    if (t > n_t0 + 2*M_PI*V/(_g*abs(n_manouver))/2){
        n_roll = 0;
    }

    gamma += isDoubleEqualToZero(n_roll) ? 0 : atan(n_roll/n_yv);
    n_yv = sqrt(pow(n_yv,2)+pow(n_roll,2));

    V += (n_xv - sin(teta.getValue()))*_g*dt;
    teta += (n_yv*cos(gamma.getValue())-cos(teta.getValue()))*_g/V*dt;
    psi += -n_yv*sin(gamma.getValue())*_g/(V*cos(teta.getValue()))*dt;
    x += V*cos(teta.getValue())*cos(psi.getValue())*dt;
    y += V*sin(teta.getValue())*dt;
    z += -V*cos(teta.getValue())*sin(psi.getValue())*dt;
}

double LA::getX()
{
    return x;
}

double LA::getY()
{
    return y;
}

double LA::getZ()
{
    return z;
}

double LA::GetV()
{
    return V;
}

double LA::GetNy()
{
    return n_yv;
}

double LA::GetNx()
{
    return n_xv;
}

Angle LA::getTeta()
{
    return teta;
}

Angle LA::getPsi()
{
    return psi;
}

Angle LA::getGamma()
{
    return gamma;
}
