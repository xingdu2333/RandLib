#include "BrownianMotion.h"

BrownianMotion::BrownianMotion(double deltaT) :
    StochasticProcess(deltaT),
    sqrtDt(std::sqrt(dt))
{
}

void BrownianMotion::nextImpl()
{
    currentValue += NormalRand::variate(0, sqrtDt);
}

void BrownianMotion::nextImpl(double deltaT)
{
    currentValue += NormalRand::variate(0, std::sqrt(deltaT));
}

double BrownianMotion::MeanImpl(double) const
{
    return currentValue;
}

double BrownianMotion::VarianceImpl(double t) const
{
    return t - currentTime;
}

double BrownianMotion::QuantileImpl(double t, double p) const
{
    return NormalRand::quantile(p, currentValue, std::sqrt(t - currentTime));
}