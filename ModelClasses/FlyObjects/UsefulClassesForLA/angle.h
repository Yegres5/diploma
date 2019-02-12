#ifndef ANGLE_H
#define ANGLE_H


class Angle
{
public:
    Angle(double value = 0);
    void incValue(const double incVal);
    double getValue() const;
    Angle operator=(const double eqVal);
    Angle& operator+=(const double incVal);
    bool check;
private:
    double value;
};

#endif // ANGLE_H
