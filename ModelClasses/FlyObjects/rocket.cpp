#include "rocket.h"
#include <QtMath>
#include <QDebug>

#define isDoubleEqualToZero(x) ( fabs(x) < 0.1e-5)

static QVector<QVector<double>> standartAtmosphere = {  {3000,  328.6, 0.9093},
                                                        {4000,  324.6, 0.8194},
                                                        {5000,  320.6, 0.7365},
                                                        {6000,  316.5, 0.6601},
                                                        {7000,  312.3, 0.59},
                                                        {8000,  308.1, 0.5258},
                                                        {9000,  303.9, 0.4671},
                                                        {10000, 299.6, 0.4135},
                                                        {11000, 295.2, 0.3648},
                                                        {12000, 295.1, 0.3119},
                                                        {14000, 295.1, 0.2279},
                                                        {16000, 295.1, 0.1665},
                                                        {18000, 295.1, 0.1216},
                                                        {20000, 295.1, 0.1216},
                                                        {24000, 297.7, 0.0889},
                                                        {28000, 300.4, 0.0469},
                                                     };

Rocket::Rocket(double x, double y, double z, double V, double teta, double psi, LA *target, double Ky, double Kz, double max_distance_to_target, double maxAngle, double startHight,
               double l, double d, double Ae, double Sm, double Ln, double Nmax,
               const char *name)
      :x(x),y(y),z(z),V(V),n_xv(0),n_yv(0),teta(teta/180*M_PI),psi(psi/180*M_PI),gamma(0),
       target(target),
       Ky(Ky),Kz(Kz),
       n_y_max(Nmax),
       max_distance_to_target(max_distance_to_target),
       max_angleLineOfSight(maxAngle),
       startHight(startHight),
       l(l),d(d),Ae(Ae),Sm(Sm),Ln(Ln)
{
    setObjectName(name);

    this->teta.check = false;
    this->gamma.check = false;

    TargetCoor = toSpeedCoordinateSystem(QVector<double> ({{target->getX(),target->getY(),target->getZ()}}));
    r = sqrt(pow(TargetCoor[0],2)+pow(TargetCoor[2],2));
    r_XY = sqrt(pow(TargetCoor[0],2)+pow(TargetCoor[1],2));
    distance_to_target = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2) + pow(TargetCoor[2],2));
}

double Rocket::p()
{
    double p = 0;
    for(auto& i : standartAtmosphere){
        if (i.first() <= y + startHight){
            p = i.at(2);
        }else{
            return p;
        }
    }
    return 0;
}

double Rocket::Mc()
{
    double p = 0;
    for(auto& i : standartAtmosphere){
        if (i.first() < y + startHight){
            p = i.at(1);
        }else{
            return p;
        }
    }
    return 0;
}

void Rocket::GravityCompensation()
{
    QVector<double> compens = {0,1,0};
    compens = toTrajectoryCoordinateSystem(compens);

    gamma += isDoubleEqualToZero(compens[2]) ? 0 : atan(compens[2]/
                                            sqrt(pow(compens[0],2)+pow(compens[1],2)));

    compens = {0,1,0};
    compens = toTrajectoryCoordinateSystem(compens);
    n_xv = compens[0];
    n_yv = compens[1];
}

void Rocket::CalculateTargetPosition()
{
    TargetCoor = toSpeedCoordinateSystem(QVector<double> ({target->getX()-x,
                                                           target->getY()-y,
                                                           target->getZ()-z}));
}

void Rocket::CalculateTargetSpeed()
{
    TargetSpeed = {target->GetV()*qCos(target->getTeta().getValue())*qCos(target->getPsi().getValue()),
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
                                                     atan2(-TargetSpeed[2],TargetSpeed[0])};
    std::copy(toSpherical->begin(),toSpherical->end(),TargetSpeed.begin());
    delete toSpherical;
}

void Rocket::CalculateNyPN()
{
    QVector<double> TargetSpeedXY = {TargetSpeed[0]*cos(TargetSpeed[1])*cos(TargetSpeed[2]),
                                    TargetSpeed[0]*sin(TargetSpeed[1])};

    TargetSpeedXY = {sqrt(pow(TargetSpeedXY[0],2) + pow(TargetSpeedXY[1],2)),
                     atan2(TargetSpeedXY[1],TargetSpeedXY[0])};

    double sigma_R_XY = -atan2(TargetCoor[1],TargetCoor[0]);
    double sigma_T_XY = TargetSpeedXY[1] - atan2(TargetCoor[1],TargetCoor[0]);

    r_XY = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2));

    double d_lambda_XY = (TargetSpeedXY[0]*sin(sigma_T_XY) - V*sin(sigma_R_XY))/r_XY;
    double W_XY = Ky*V*d_lambda_XY;
    n_pitch = W_XY/_g;
}

void Rocket::CalculateNzPN()
{
    QVector<double> TargetSpeedXZ = {TargetSpeed[0]*cos(TargetSpeed[1]),
                                    TargetSpeed[2]};

    double sigma_R = -atan2(-TargetCoor[2],TargetCoor[0]);
    double sigma_T = TargetSpeedXZ[1] - atan2(-TargetCoor[2],TargetCoor[0]);
    r = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[2],2));
    double d_lambda = (TargetSpeedXZ[0]*sin(sigma_T) - V*sin(sigma_R))/r;
    double W = -Kz*V*d_lambda;
    n_roll = W/_g;
}

void Rocket::SummarizeAllOverload()
{
    n_yv += n_pitch;
    gamma += isDoubleEqualToZero(n_roll) ? 0 : atan(n_roll/n_yv);
    n_yv = sqrt(pow(n_yv,2) + pow(n_roll,2))*(n_yv > 0 ? 1 : -1);
}

void Rocket::CheckMaxNy()
{
    n_yv = (abs(n_yv) > n_y_max) ? n_y_max*(n_yv > 0 ? 1 : -1) : n_yv;
}

void Rocket::EquationsOfMotion(double dt)
{
    V += (n_xv - sin(teta.getValue()))*_g*dt;
    teta += (n_yv*cos(gamma.getValue())-cos(teta.getValue()))*_g/V*dt;
    psi += -n_yv*sin(gamma.getValue())*_g/(V*cos(teta.getValue()))*dt;
    x += V*cos(teta.getValue())*cos(psi.getValue())*dt;
    y += V*sin(teta.getValue())*dt;
    z += -V*cos(teta.getValue())*sin(psi.getValue())*dt;
}

void Rocket::CheckTargetGetReached()
{
    if (distance_to_target > max_distance_to_target){
        distance_to_target = sqrt(pow(TargetCoor[0],2) + pow(TargetCoor[1],2) + pow(TargetCoor[2],2));
    }else{
        emit targetGetReached(0);
    }
}

void Rocket::CalculatingDragForce()
{
    double Cd_body = 0.053*(l/d)*pow( M()/(q()*l), 0.2);

    double Cd_base = 0.25/M();

    double Cd_base_powered = (1-Ae/Sm)*(0.25/M());

    double Cd_body_wave = (1.59+1.83/pow(M(),2))*pow( (tan(0.5*Ln/d)),-1.89);

    double Cx0 = Cd_body + Cd_base + Cd_base_powered + Cd_body_wave;

    //m S
    double drag_force = Cx0*0.026*q();
    double drag_acceleration = drag_force/175;
    n_xv -= drag_acceleration/_g;
}

void Rocket::CheckMaxAngle()
{
    QList<double> Vvec({V*sin(M_PI_2-teta.getValue())*cos(psi.getValue()),
            V*cos(M_PI_2-teta.getValue()),
            -V*sin(M_PI_2-teta.getValue())*sin(psi.getValue())});

    QList<double> TargetCoor =     QList<double> ({ target->getX()-x,
                                                    target->getY()-y,
                                                    target->getZ()-z});

    angleOfSight = qAcos( (Vvec[0]*TargetCoor[0]+Vvec[1]*TargetCoor[1]+Vvec[2]*TargetCoor[2])/
            (sqrt(pow(Vvec[0],2)+pow(Vvec[1],2)+pow(Vvec[2],2))*sqrt(pow(TargetCoor[0],2)+pow(TargetCoor[1],2)+pow(TargetCoor[2],2))));

    if (angleOfSight > (max_angleLineOfSight)/180*M_PI){
        targetGetReached(2);
    }
}

void Rocket::update(double dt)
{
    GravityCompensation();

    CalculateTargetPosition();
    CalculateTargetSpeed();

    CalculateNyPN();
    CalculateNzPN();
    SummarizeAllOverload();
    CheckMaxNy();
    CalculatingDragForce();

    CheckMaxAngle();

    n_pitch = n_yv*cos(gamma.getValue());
    n_roll = -n_yv*sin(gamma.getValue());

    EquationsOfMotion(dt);

    CheckTargetGetReached();
}

QVector<double> Rocket::toSpeedCoordinateSystem(QVector<double> vec)
{
    double teta  = -this->teta.getValue();
    double psi   = -this->psi.getValue();
    double gamma = -this->gamma.getValue();

    QVector<double> a{cos(teta)*cos(psi),                                    sin(teta),              -cos(teta)*sin(psi)};
    QVector<double> b{-cos(gamma)*sin(teta)*cos(psi)+sin(gamma)*sin(psi), cos(gamma)*cos(teta),  cos(gamma)*sin(teta)*sin(psi)+sin(gamma)*cos(psi)};
    QVector<double> c{sin(gamma)*sin(teta)*cos(psi)+cos(gamma)*sin(psi),  -sin(gamma)*cos(teta), -sin(psi)*sin(teta)*sin(gamma)+cos(psi)*cos(gamma)};

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
    double psi   = 0;
    double gamma = -this->gamma.getValue();

    QVector<QVector<double>> arr{
                        {cos(teta)*cos(psi),                                    sin(teta),              -cos(teta)*sin(psi)},
                        {-cos(gamma)*sin(teta)*cos(psi)+sin(gamma)*sin(psi), cos(gamma)*cos(teta),  cos(gamma)*sin(teta)*sin(psi)+sin(gamma)*cos(psi)},
                        {sin(gamma)*sin(teta)*cos(psi)+cos(gamma)*sin(psi),  -sin(gamma)*cos(teta), -sin(psi)*sin(teta)*sin(gamma)+cos(psi)*cos(gamma)}};

    QVector<double> Result(3);

    for(int i = 0; i < arr.at(0).size(); i++){
        for(int j = 0; j < vec.size(); j++){
            Result[i] += vec.at(j)*arr.at(j).at(i);
        }
    }
    return Result;
}
