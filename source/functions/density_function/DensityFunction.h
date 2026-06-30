#pragma once

#include <limits>

#include "source/functions/Function.hpp"
#include "source/data_structures/interval/Interval.h"


class DensityFunction: public Function<double, double> {
protected:
    DensityFunction();
    DensityFunction(const Domain<double>& domain, const Range<double>& range);
};
