#pragma once

#include <iostream>

#include "source/distributions/continuous_distributions/ContinuousRandomVariable.hpp"
#include "source/functions/density_function/uniform_density_function/UniformDensityFunction.h"


class Uniform: public ContinuousRandomVariable<double> {
public:
    Uniform(double lower, double upper);

    double calculateProbability(const Interval& interval) const override;
    double getExpectation() const override;
    double getVariance() const override;
    RandomVariable<Interval>* clone() const override;
};