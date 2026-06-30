#include <cmath>
#include <limits>
#include <stdexcept>

#include "source/data_structures/special_functions/Gamma.h"
#include "source/functions/density_function/chi_squared_density_function/ChiSquaredDensityFunction.h"

ChiSquaredDensityFunction::ChiSquaredDensityFunction(uint32_t degreesOfFreedom):
    DensityFunction(Domain<double>(), Interval(0.0, std::numeric_limits<double>::infinity())) {
    this->setDegreesOfFreedom(degreesOfFreedom);
}

void ChiSquaredDensityFunction::setDegreesOfFreedom(uint32_t degreesOfFreedom) {
    if (degreesOfFreedom == 0) throw std::logic_error("Chi-squared degrees of freedom must be >= 1");
    this->degreesOfFreedom = degreesOfFreedom;
}

double ChiSquaredDensityFunction::operator () (const double& input) const {
    if (input < 0.0) return 0.0;
    const double halfDegreesOfFreedom = static_cast<double>(this->degreesOfFreedom) / 2.0;
    if (input == 0.0) {
        if (this->degreesOfFreedom == 1) return std::numeric_limits<double>::infinity();
        if (this->degreesOfFreedom == 2) return 0.5;
        return 0.0;
    }

    const double logResult = -halfDegreesOfFreedom * std::log(2.0)
        - SpecialFunctions::logGamma(halfDegreesOfFreedom)
        + (halfDegreesOfFreedom - 1.0) * std::log(input)
        - input / 2.0;
    return std::exp(logResult);
}

Function<double, double>* ChiSquaredDensityFunction::clone() const {
    return new ChiSquaredDensityFunction(*this);
}
