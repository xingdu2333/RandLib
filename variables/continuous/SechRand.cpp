#include "SechRand.h"

SechRand::SechRand()
{

}

std::string SechRand::name()
{
    return "Hyperbolic secant";
}

double SechRand::f(double x) const
{
    return 0.5 / std::cosh(M_PI_2 * x);
}

double SechRand::F(double x) const
{
    double y = std::exp(M_PI_2 * x);
    return M_2_PI * std::atan(y);
}

double SechRand::variate() const
{
    double y = std::fabs(CauchyRand::standardVariate());
    return M_2_PI * std::log(y);
}

double SechRand::Mean() const
{
    return 0.0;
}

double SechRand::Variance() const
{
    return 1.0;
}

double SechRand::Quantile(double p) const
{
    if (p < 0 || p > 1)
        return NAN;
    double x = M_PI_2 * p;
    x = std::tan(x);
    x = std::log(x);
    return M_2_PI * x;
}

double SechRand::Median() const
{
    return 0.0;
}

double SechRand::Mode() const
{
    return 0.0;
}

double SechRand::Skewness() const
{
    return 0.0;
}

double SechRand::Kurtosis() const
{
    return 2.0;
}

double SechRand::Entropy() const
{
    return 2.0 * M_2_PI * M_CATALAN;
}
