#include "GeometricRand.h"
#include "../continuous/UniformRand.h"

GeometricRand::GeometricRand(double probability)
{
    setProbability(probability);
}

std::string GeometricRand::name()
{
    return "Geometric(" + toStringWithPrecision(getProbability()) + ")";
}

void GeometricRand::setProbability(double probability)
{
    p = probability;
    if (p < 0.0 || p > 1.0)
        p = 0.5;
    q = 1.0 - p;

    /// we use two different generators for two different cases
    /// if p < 0.2 then the tail is too heavy
    /// (probability to be in main body is less than 0.977482)
    /// thus we return highest integer less than variate from exponential distribution
    /// otherwise we choose table method
    if (p < 0.2)
    {
        W.setRate(-std::log(q));
    }
    else
    {
        table[0] = p;
        double prod = p;
        for (int i = 1; i < tableSize; ++i)
        {
            prod *= q;
            table[i] = table[i - 1] + prod;
        }
    }
}

double GeometricRand::P(int k) const
{
    return (k < 0) ? 0 : p * std::pow(q, k);
}

double GeometricRand::F(double x) const
{
    return (x < 0) ? 0 : 1 - std::pow(q, std::floor(x) + 1);
}

double GeometricRand::variateByExponential() const
{
    return std::floor(W.variate());
}

double GeometricRand::variateByTable() const
{
    double U = UniformRand::standardVariate();
    /// handle tail by recursion
    if (U > table[tableSize - 1])
        return tableSize + variateByTable();
    /// handle the main body
    int x = 0;
    while (U > table[x])
        ++x;
    return x;
}

double GeometricRand::variate() const
{
    return (p < 0.2) ? variateByExponential() : variateByTable();
}

double GeometricRand::variate(double probability)
{
    /// here we use 0.1 instead of 0.2 because log(1-p) wasn't hashed
    if (probability < 0.1)
        return std::floor(ExponentialRand::variate(-std::log(1 - probability)));

    double U = UniformRand::standardVariate();
    int x = 0;
    double prod = probability, sum = prod, q = 1 - probability;
    while (U > sum) {
        prod *= q;
        sum += prod;
        ++x;
    }
    return x;
}

void GeometricRand::sample(QVector<double> &outputData) const
{
    if (p < 0.2) {
        for (double &var : outputData)
            var = variateByExponential();
    }
    else {
        for (double &var : outputData)
            var = variateByTable();
    }
}

double GeometricRand::Mean() const
{
    return q / p;
}

double GeometricRand::Variance() const
{
    return q / (p * p);
}

std::complex<double> GeometricRand::CF(double t) const
{
    std::complex<double> y(0, t);
    y = std::exp(y);
    y *= p;
    y = 1.0 - y;
    return (1.0 - p) / y;
}

double GeometricRand::Median() const
{
    return std::floor(-M_LN2 / std::log(q));
}

double GeometricRand::Mode() const
{
    return 0.0;
}

double GeometricRand::Skewness() const
{
    return (2 - p) / std::sqrt(q);
}

double GeometricRand::ExcessKurtosis() const
{
    return p * p / q + 6;
}

double GeometricRand::Entropy() const
{
    double a = -q * std::log(q);
    double b = -p * std::log(p);
    return (a + b) / (M_LN2 * p);
}

bool GeometricRand::checkValidity(const QVector<double> &sample)
{
    for (int var : sample) {
        if (var < 0)
            return false;
    }
    return true;
}

bool GeometricRand::fit_MLE(const QVector<double> &sample)
{
    if (!checkValidity(sample))
        return false;
    setProbability(1.0 / (RandMath::sampleMean(sample) + 1));
    return true;
}

bool GeometricRand::fit_MM(const QVector<double> &sample)
{
    return fit_MLE(sample);
}

bool GeometricRand::fitProbability_Bayes(const QVector<double> &sample, BetaRand &priorDistribution)
{
    int n = sample.size();
    double alpha = priorDistribution.getAlpha();
    double beta = priorDistribution.getBeta();
    priorDistribution.setParameters(alpha + n, beta + RandMath::sum(sample));
    setProbability(priorDistribution.Mean());
    return true;
}
