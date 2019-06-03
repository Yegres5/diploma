#include "la.h"
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>

#define isDoubleEqualToZero(x) ( fabs(x) < 0.1e-5)

LA::LA()
{}

LA::LA(double x, double y, double z, double V, double n_xv,
       double n_yv, double teta, double psi, double gamma, double n_manouver, double tManouver, double deltaAngle, double angleForSnake, const char *name):
        x(x),y(y),z(z),V(V),n_xv(n_xv),n_yv(n_yv),n_roll(0),teta(teta),psi(psi),gamma(gamma/180*M_PI),
        n_manouver(n_manouver),tManouver(tManouver),t(0),deltaAngle(deltaAngle),startSnake(false),manouver_flag(false),angleForSnake(angleForSnake),increasedAngleForShake(false)
{    
    setObjectName(name);
}

void LA::update(double dt)
{
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
    t += dt;

    if ((t > tManouver) && (!startSnake)){
        startSnake = makeManouverToAngle(deltaAngle);
    }
    if (startSnake){
        manouver_flag = makeManouverToAngle(deltaAngle + angleForSnake/180*M_PI);
        if (manouver_flag){
            angleForSnake = -angleForSnake;
        }
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

bool LA::makeManouverToAngle(double angle)
{
    double tempAngle = psi.getValue() - angle;

    while (tempAngle > M_PI){
        tempAngle -= 2*M_PI;
    }

    while (tempAngle < -M_PI){
        tempAngle += 2*M_PI;
    }

    if (abs(tempAngle) > 5./180*M_PI){
        if (tempAngle > 0){
            n_roll = n_manouver;
        }else{
            n_roll = -n_manouver;  
        }
        return false;
    }else{
        n_roll = 0;
        return true;
    }
}


