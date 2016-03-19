#ifndef GEOMETRICRAND_H
#define GEOMETRICRAND_H

#include "DiscreteDistribution.h"
#include "../continuous/ExponentialRand.h"
#include "../continuous/BetaRand.h"
#include <functional>

/**
 * @brief The GeometricRand class
 */
class RANDLIBSHARED_EXPORT GeometricRand : public DiscreteDistribution
{
    double p, q;

    static constexpr int tableSize = 16;
    // TODO: don't storage both variables (including tableSize)
    double table[tableSize];
    ExponentialRand W;

public:
    explicit GeometricRand(double probability = 0.5);
    std::string name() override;

    void setProbability(double probability);
    inline double getProbability() const { return p; }

    double P(int k) const override;
    double F(double x) const override;
    double variate() const override;
    static double variate(double probability);

    void sample(QVector<double> &outputData) const override;

private:
    double variateByExponential() const;
    double variateByTable() const;

public:
    double Mean() const override;
    double Variance() const override;

    std::complex<double> CF(double t) const override;

    double Median() const override;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

    inline double Entropy() const;
    
    bool checkValidity(const QVector<double> &sample);

    bool fit_MLE(const QVector<double> &sample);
    bool fit_MM(const QVector<double> &sample);
    bool fitProbability_Bayes(const QVector<double> &sample, BetaRand &priorDistribution);
};

#endif // GEOMETRICRAND_H
