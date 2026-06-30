#pragma once

#include <iostream>

#include "source/distributions/continuous_distributions/ContinuousRandomVariable.hpp"


class Exponential: public ContinuousRandomVariable<double> {
public:
    Exponential(double lambda);

    double calculateProbability(const Interval& interval) const override;
    double getExpectation() const override;
    double getVariance() const override;
    RandomVariable<Interval>* clone() const override;
};