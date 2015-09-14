#ifndef LAPLACERAND_H
#define LAPLACERAND_H

#include "ContinuousRand.h"
#include "ExponentialRand.h"
#include "../BasicRandGenerator.h"

/**
 * @brief The LaplaceRand class
 */
class RANDLIBSHARED_EXPORT LaplaceRand : public ContinuousRand
{
    double mu, b;
    double bInv; /// 1 / b

public:
    LaplaceRand(double location = 0, double scale = 1);
    virtual std::string name() override;

    void setLocation(double location);
    void setScale(double scale);
    inline double getLocation() const { return mu; }
    inline double getScale() const { return b; }

    double f(double x) const override;
    double F(double x) const override;
    double variate() const override;
    static double variate(double location, double scale);

    double Mean() const override { return mu; }
    double Variance() const override { return 2 * b * b; }

    double Median() const override;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;
    
    std::complex<double> CF(double t) const override;

    bool fitToData(const QVector<double> &sample);
};

#endif // LAPLACERAND_H
