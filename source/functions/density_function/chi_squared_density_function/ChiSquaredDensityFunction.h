#pragma once

#include <cstdint>

#include "source/functions/density_function/DensityFunction.h"


class ChiSquaredDensityFunction: public DensityFunction {
private:
    uint32_t degreesOfFreedom = 0;
    void setDegreesOfFreedom(uint32_t degreesOfFreedom);

public:
    ChiSquaredDensityFunction(uint32_t degreesOfFreedom);

    double operator () (const double& input) const override;
    Function<double, double>* clone() const override;
};
