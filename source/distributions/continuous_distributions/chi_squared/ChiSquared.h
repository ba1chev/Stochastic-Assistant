#pragma once

#include <cstdint>
#include "source/distributions/continuous_distributions/ContinuousRandomVariable.hpp"


class ChiSquared: public ContinuousRandomVariable<double> {
public:
    ChiSquared(uint32_t degreesOfFreedom);

    double calculateProbability(const Interval& interval) const override;
    double getExpectation() const override;
    double getVariance() const override;
    RandomVariable<Interval>* clone() const override;
};
