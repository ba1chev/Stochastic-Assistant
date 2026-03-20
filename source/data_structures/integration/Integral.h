#pragma once

#include "source/functions/Function.hpp"
#include "source/data_structures/integration/IntegrationApproximation.h"
#include <stdexcept>


class Integral {
protected:
    const Function<double, double>* function = nullptr;
    IntegrationApproximation approximation = IntegrationApproximation::None;

    void setFunctionPtr(const Function<double, double>* function);
    void setIntegrationApproximation(IntegrationApproximation approximation);

public:
    Integral(const Function<double, double>* function, IntegrationApproximation approximation);

    virtual double integrate(double lower, double upper) const = 0;
    virtual Integral* clone() const = 0;
    virtual ~Integral() = default;
};
