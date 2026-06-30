#pragma once

#include <cstdint>
#include "source/distributions/continuous_distributions/ContinuousRandomVariable.hpp"


class StudentT: public ContinuousRandomVariable<double> {
public:
    StudentT(uint32_t degreesOfFreedom);

    double calculateProbability(const Interval& interval) const override;
    double getExpectation() const override;
    double getVariance() const override;
    RandomVariable<Interval>* clone() const override;
};
