#include "ZetaRand.h"
#include "../continuous/UniformRand.h"
#include "../continuous/ParetoRand.h"

ZetaRand::ZetaRand(double exponent)
{
    setExponent(exponent);
}

std::string ZetaRand::name()
{
    return "Zeta(" + toStringWithPrecision(getExponent()) + ")";
}

void ZetaRand::setExponent(double exponent)
{
    s = exponent;
    /// sanity check
    if (s <= 1.0)
        s = 2.0; /// default value
    sm1 = s - 1.0;
    zetaSInv = 1.0 / RandMath::zetaRiemann(s);
    b = 1.0 - std::pow(2.0, -sm1);
}

double ZetaRand::P(int k) const
{
    if (k < 1)
        return 0.0;
    return zetaSInv / std::pow(k, s);
}

double ZetaRand::F(double x) const
{
    if (x < 1.0)
        return 0.0;
    int k = static_cast<int>(std::floor(x));
    return zetaSInv * RandMath::harmonicNumber(s, k);
}

double ZetaRand::variate() const
{
    /// Luc Devroye, p. 551
    /// rejection sampling from rounded down Pareto distribution
    int iter = 0;
    do {
        double X = std::floor(ParetoRand::standardVariate(sm1));
        double T = std::pow(1.0 + 1.0 / X, sm1);
        double V = UniformRand::standardVariate();
        /// there was typo in the book - '<=' instead of '>'
        if (V * X * (T - 1) <= b * T )
            return X;
    } while (++iter < 1e9);
    return NAN; /// doesn't work
}

double ZetaRand::Mean() const
{
    if (s <= 2)
        return INFINITY;
    return zetaSInv * RandMath::zetaRiemann(sm1);
}

double ZetaRand::Variance() const
{
    if (s <= 3)
        return INFINITY;
    double y = Mean();
    double z = zetaSInv * RandMath::zetaRiemann(s - 2);
    return z - y * y;
}

double ZetaRand::Mode() const
{
    return 1.0;
}

double ZetaRand::Skewness() const
{
    if (s <= 4)
        return INFINITY;

    double z1 = RandMath::zetaRiemann(sm1), z1Sq = z1 * z1;
    double z2 = RandMath::zetaRiemann(s - 2);
    double z3 = RandMath::zetaRiemann(s - 3);
    double z = 1.0 / zetaSInv, zSq = z * z;

    double numerator = zSq * z3;
    numerator -= 3 * z2 * z1 * z;
    numerator += 2 * z1 * z1Sq;

    double denominator = z * z2 - z1Sq;
    denominator = std::pow(denominator, 1.5);
    denominator *= zSq;

    return numerator / denominator;
}
