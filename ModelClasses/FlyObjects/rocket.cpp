#include "rocket.h"
#include <QDebug>
#include <QtMath>
#include <QFile>

#define isDoubleEqualToZero(x) ( fabs(x) < 0.1e-5)

double CalcAngle(double dx, double dy);

Rocket::Rocket(double x, double y, double z, double V, double n_xv,
               double n_yv, double teta, double psi, double gamma, LA *target, double K,
               const char *name)
      :x(x),y(y),z(z),V(V),n_xv(n_xv),n_yv(n_yv),teta(teta),psi(psi),gamma(gamma),
       target(target),
       K(K)
{
    setObjectName(name);

    this->teta.check = false;//CHANGE
    this->gamma.check = false;//CHANGE
    r_explode = 11; //CHANGE

    TargetCoor = toSpeedCoordinateSystem(QVector<double> ({{target->getX(),target->getY(),target->getZ()}}));
    r = sqrt(pow(TargetCoor[0],2)+pow(TargetCoor[2],2));
    r_XY = sqrt(pow(TargetCoor[0],2)+pow(TargetCoor[1],2));
    distance_to_target = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2) + pow(TargetCoor[2],2));
}

double Rocket::getNy()
{
    return n_yv;
}

void Rocket::update(double dt)
{
    TargetCoordinatesInSpeed = toSpeedCoordinateSystem(QVector<double> ({{target->getX()-x,
                                                           target->getY()-y,
                                                           target->getZ()-z}}));
//Gravity compensation
    QVector<double> grav = {0,1,0};
    grav = toTrajectoryCoordinateSystem(grav);

    gamma += isDoubleEqualToZero(grav[2]) ? 0 : atan(grav[2]/
                                            sqrt(pow(grav[0],2)+pow(grav[1],2)));



    grav = {0,1,0};
    grav = toTrajectoryCoordinateSystem(grav);
    n_xv = grav[0];
    n_yv = grav[1];

//Calculation target position, QVector of target speed, QVector of self speed
    TargetCoor = toSpeedCoordinateSystem(QVector<double> ({{target->getX()-x,
                                                           target->getY()-y,
                                                           target->getZ()-z}}));


    QVector<double> TargetSpeed = {target->GetV()*qCos(target->getTeta().getValue())*qCos(target->getPsi().getValue()),
                                  target->GetV()*qSin(target->getTeta().getValue()),
                                  -target->GetV()*qCos(target->getTeta().getValue())*qSin(target->getPsi().getValue())};

    TargetSpeed = toSpeedCoordinateSystem(TargetSpeed);

    QVector<double>* toSpherical = new QVector<double>{sqrt(pow(TargetSpeed[0],2)+
                                                          pow(TargetSpeed[1],2)+
                                                          pow(TargetSpeed[2],2)),
                                                     qAcos(sqrt(pow(TargetSpeed[0],2)+
                                                                pow(TargetSpeed[2],2))/
                                                           sqrt(pow(TargetSpeed[0],2)+
                                                                pow(TargetSpeed[1],2)+
                                                                pow(TargetSpeed[2],2)))*(TargetSpeed[1] < 0 ? -1 : 1),
                                                     CalcAngle(TargetSpeed[0],-TargetSpeed[2])};
    std::copy(toSpherical->begin(),toSpherical->end(),TargetSpeed.begin());
    delete toSpherical;

    QVector<double> SelfSpeed = {V*qCos(teta.getValue())*qCos(psi.getValue()),
                                V*qSin(teta.getValue()),
                                -V*qCos(teta.getValue())*qSin(psi.getValue())};
    SelfSpeed = toSpeedCoordinateSystem(SelfSpeed);

    toSpherical = new QVector<double>{sqrt(pow(SelfSpeed[0],2)+
                                          pow(SelfSpeed[1],2)+
                                          pow(SelfSpeed[2],2)),
                                     qAcos(sqrt(pow(SelfSpeed[0],2)+
                                                pow(SelfSpeed[2],2))/
                                           sqrt(pow(SelfSpeed[0],2)+
                                           pow(SelfSpeed[1],2)+
                                           pow(SelfSpeed[2],2))),
                                     CalcAngle(SelfSpeed[0],-SelfSpeed[2])};

    std::copy(toSpherical->begin(),toSpherical->end(),SelfSpeed.begin());
    delete toSpherical;

//FOR XY (pitch)
    QVector<double> SelfSpeedXY = {SelfSpeed[0], 0};

    QVector<double> TargetSpeedXY = {TargetSpeed[0]*cos(TargetSpeed[1])*cos(TargetSpeed[2]),
                                    TargetSpeed[0]*sin(TargetSpeed[1])};

    TargetSpeedXY = {sqrt(pow(TargetSpeedXY[0],2) + pow(TargetSpeedXY[1],2)),
                     CalcAngle(TargetSpeedXY[0],TargetSpeedXY[1])};

    double sigma_R_XY = SelfSpeedXY[1] - CalcLambdaYX();
    double sigma_T_XY = TargetSpeedXY[1] - CalcLambdaYX();

    r_XY = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2));

    double d_lambda_XY = (TargetSpeedXY[0]*sin(sigma_T_XY) - SelfSpeedXY[0]*sin(sigma_R_XY))/r_XY;
    //qDebug("---dLambda = %5.f", d_lambda_XY);
    double W_XY = K*V*d_lambda_XY;
    double n_pitch = W_XY/_g;



// FOR XZ (roll)
    QVector<double> SelfSpeedXZ = {SelfSpeed[0]*cos(SelfSpeed[1]),
                                  SelfSpeed[2]};
    QVector<double> TargetSpeedXZ = {TargetSpeed[0]*cos(TargetSpeed[1]),
                                    TargetSpeed[2]};

    double sigma_R = SelfSpeedXZ[1] - CalcLambdaXZ();
    double sigma_T = TargetSpeedXZ[1] - CalcLambdaXZ();
    r = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[2],2));
    double d_lambda = (TargetSpeedXZ[0]*sin(sigma_T) - SelfSpeedXZ[0]*sin(sigma_R))/r;
    double W = -K*V*d_lambda;
    double n_roll = W/_g;

//Summ of gravity, roll and pitch
    n_yv += n_pitch;

    gamma += isDoubleEqualToZero(n_roll) ? 0 : atan(n_roll/n_yv);
    n_yv = sqrt(pow(n_yv,2) + pow(n_roll,2))*(n_yv > 0 ? 1 : -1);
    n_roll = 0;

    if (n_yv > n_y_max){
        n_y_max = n_yv;
    }

//Equations of motion
    V += (n_xv - sin(teta.getValue()))*_g*dt;
    teta += (n_yv*cos(gamma.getValue())-cos(teta.getValue()))*_g/V*dt;
    psi += -n_yv*sin(gamma.getValue())*_g/(V*cos(teta.getValue()))*dt;
    x += V*cos(teta.getValue())*cos(psi.getValue())*dt;
    y += V*sin(teta.getValue())*dt;
    z += -V*cos(teta.getValue())*sin(psi.getValue())*dt;

// Target get reached by rocket
    if (distance_to_target > 100){
        distance_to_target = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2) + pow(TargetCoor[2],2));
    }else{
        emit targetGetReached();
    }
}

double Rocket::GetSigmaT()
{
    double dx = sqrt(pow(target->getX()-x,2)+pow(target->getZ()-z,2));
    double dy = target->getY()-y;

    if (isDoubleEqualToZero(dx)){
        if (dy>0){
            return M_PI_2;
        }else{
            return -M_PI_2;
        }
    }

    if (isDoubleEqualToZero(dy)){
        if (dx>0){
            return 0;
        }
    }

    return atan(dy/dx);
}

double Rocket::CalcLambdaXZ()
{
    double dx = TargetCoor[0];  //X
    double dy = -TargetCoor[2];  //Z

    return CalcAngle(dx,dy);
}

double Rocket::CalcLambdaYX()
{
    double dx = TargetCoor[0];  //X
    double dy = TargetCoor[1];  //Z

    return CalcAngle(dx,dy);
}

QVector<double> Rocket::CalcV_XYprojection(QVector<double> speed)
{
  QVector<double> resultSpeed(2);
  resultSpeed[0] = sqrt(pow(speed[0]*cos(speed[1])*cos(speed[2]),2)+
                        pow(speed[0]*sin(speed[1]),2));
  resultSpeed[1] = CalcAngle(speed[0]*cos(speed[1])*cos(speed[2]),
                             speed[0]*sin(speed[1]));
  return resultSpeed;
}

double Rocket::GetVT()
{
  return target->GetV();
}

double Rocket::CalcXY_Angle()
{
    double dx = target->getX()-x;
    double dy = target->getY()-y;

    if (isDoubleEqualToZero(dx)){
        if (dy>0){
            return M_PI_2;
        }else{
            return M_PI+M_PI_2;
        }
    }

    if (isDoubleEqualToZero(dy)){
        if (dx>0){
            return 0;
        }else{
            return M_PI;
        }
    }

    if (dx < 0){
        if(dy < 0){
            return atan(dy/dx)+M_PI;
        }else{
            return atan(dy/dx)+M_PI;
        }
    }else{
        if (dy < 0){
            return 2*M_PI-atan(dy/dx);
        }else{
            return atan(dy/dx);
        }
    }
}

bool Rocket::checkForRoll()
{

    if(teta.getValue() > M_PI_2){
        teta = M_PI-teta.getValue();
        psi.incValue(M_PI);
        return true;
    }else{
        if(teta.getValue() < -M_PI_2){
            teta = -M_PI-teta.getValue();
            psi.incValue(M_PI);
            return true;
        }
    }
    return false;

}

QVector<double> Rocket::toSpeedCoordinateSystem(QVector<double> vec)
{
    double teta  = -this->teta.getValue();
    double psi   = -this->psi.getValue();
    double gamma = -this->gamma.getValue();

    QVector<double> a{qCos(teta)*qCos(psi),                                    qSin(teta),              -qCos(teta)*qSin(psi)};
    QVector<double> b{-qCos(gamma)*qSin(teta)*qCos(psi)+qSin(gamma)*qSin(psi), qCos(gamma)*qCos(teta),  qCos(gamma)*qSin(teta)*qSin(psi)+qSin(gamma)*qCos(psi)};
    QVector<double> c{qSin(gamma)*qSin(teta)*qCos(psi)+qCos(gamma)*qSin(psi),  -qSin(gamma)*qCos(teta), -qSin(psi)*qSin(teta)*qSin(gamma)+qCos(psi)*qCos(gamma)};

    QVector<QVector<double>> arr{a,b,c};

    QVector<double> Result(3);

    for(int i = 0; i < arr.at(0).size(); i++){
        for(int j = 0; j < vec.size(); j++){
            Result[i] += vec.at(j)*arr.at(j).at(i);
        }
    }
    return Result;
}

QVector<double> Rocket::toTrajectoryCoordinateSystem(QVector<double> vec)
{
    double teta  = -this->teta.getValue();
    double psi   = 0;//-this->psi.getValue();
    double gamma = -this->gamma.getValue();

    QVector<QVector<double>> arr{
                        {qCos(teta)*qCos(psi),                                    qSin(teta),              -qCos(teta)*qSin(psi)},
                        {-qCos(gamma)*qSin(teta)*qCos(psi)+qSin(gamma)*qSin(psi), qCos(gamma)*qCos(teta),  qCos(gamma)*qSin(teta)*qSin(psi)+qSin(gamma)*qCos(psi)},
                        {qSin(gamma)*qSin(teta)*qCos(psi)+qCos(gamma)*qSin(psi),  -qSin(gamma)*qCos(teta), -qSin(psi)*qSin(teta)*qSin(gamma)+qCos(psi)*qCos(gamma)}};

    QVector<double> Result(3);

    for(int i = 0; i < arr.at(0).size(); i++){
        for(int j = 0; j < vec.size(); j++){
            Result[i] += vec.at(j)*arr.at(j).at(i);
        }
    }
    return Result;
}

double CalcAngle(double dx, double dy){
    if (isDoubleEqualToZero(dx)){
        if (dy>0){
            return M_PI_2;
        }else{
            return M_PI+M_PI_2;
        }
    }

    if (isDoubleEqualToZero(dy)){
        if (dx>0){
            return 0;
        }else{
            return M_PI;
        }
    }

    if (dx < 0){
        if(dy < 0){
            return atan(dy/dx)+M_PI;
        }else{
            return atan(dy/dx)+M_PI;
        }
    }else{
        if (dy < 0){
            return 2*M_PI+atan(dy/dx);
        }else{
            return atan(dy/dx);
        }
    }
}
