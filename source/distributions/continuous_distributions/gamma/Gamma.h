#pragma once

#include "source/distributions/continuous_distributions/ContinuousRandomVariable.hpp"


class Gamma: public ContinuousRandomVariable<double> {
public:
    Gamma(double alpha, double beta);

    double calculateProbability(const Interval& interval) const override;
    double getExpectation() const override;
    double getVariance() const override;
    RandomVariable<Interval>* clone() const override;
};
