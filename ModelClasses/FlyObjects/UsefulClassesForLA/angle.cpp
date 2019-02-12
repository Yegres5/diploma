#include "angle.h"
#include "QtMath"

Angle::Angle(double value):check(true),value(value)
{}

void Angle::incValue(const double incVal)
{
    value += incVal;
    if (check){
    while(value>=M_PI*2)
        value -= M_PI*2;
    while(value<0)
        value += M_PI*2;
    }
}
double Angle::getValue() const
{
    return value;
}

Angle Angle::operator=(const double eqVal)
{
    value = eqVal;
    return *this;
}

Angle &Angle::operator+=(const double incVal)
{
    this->incValue(incVal);
    return *this;
}
