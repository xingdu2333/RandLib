#include "RademacherRand.h"

RademacherRand::RademacherRand()
{
}

double RademacherRand::P(int k) const
{
    if (k == -1 || k == 1)
        return 0.5;
    return 0;
}

double RademacherRand::F(double x) const
{
    if (x < -1)
        return 0;
    if (x < 1)
        return 0.5;
    return 1;
}

int RademacherRand::value()
{
    if ((signed)B.getRand() < 0)
        return -1;
    return 1;
}
