#include "source/data_structures/integration/Integral.h"

Integral::Integral(const Function<double, double>* function, IntegrationApproximation approximation) {
    this->setFunctionPtr(function);
    this->setIntegrationApproximation(approximation);
}

void Integral::setFunctionPtr(const Function<double, double>* function) {
    if (!function) throw std::runtime_error("Nullptr detected");
    this->function = function;
}

void Integral::setIntegrationApproximation(IntegrationApproximation approximation) {
    if (approximation == IntegrationApproximation::None) 
        throw std::runtime_error("Approximation must be different from None");
    this->approximation = approximation;
}