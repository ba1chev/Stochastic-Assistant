#pragma once

#include "source/functions/density_function/DensityFunction.h"


class GammaDensityFunction: public DensityFunction {
private:
    double alpha = 0.0;
    double beta = 0.0;

    void setAlpha(double alpha);
    void setBeta(double beta);

public:
    GammaDensityFunction(double alpha, double beta);

    double operator () (const double& input) const override;
    Function<double, double>* clone() const override;
};
