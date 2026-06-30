#pragma once

#include "source/functions/Function.hpp"
#include "source/data_structures/integration/Integral.h"


class TrapezoidalRuleIntegral: public Integral {
private:
    size_t countOfSubIntervals = 0;
    void setCountOfNonSubIntervals(size_t countOfSubIntervals);

public:
    TrapezoidalRuleIntegral(const Function<double, double>* function, size_t countOfSubIntervals);
    
    double integrate(double lower, double upper) const override;
    Integral* clone() const override;
};