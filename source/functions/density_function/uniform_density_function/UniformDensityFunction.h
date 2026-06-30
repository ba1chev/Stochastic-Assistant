#pragma once

#include "source/data_structures/interval/Interval.h"
#include "source/functions/density_function/DensityFunction.h"


class UniformDensityFunction: public DensityFunction {
private:
    Interval interval;
    void setInterval(const Interval& interval);

public:
    UniformDensityFunction(const Interval& interval);

    double operator () (const double& input) const override;
    Function<double, double>* clone() const override;
};
