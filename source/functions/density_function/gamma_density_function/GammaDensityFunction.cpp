#include <cmath>
#include <limits>
#include <stdexcept>

#include "source/data_structures/special_functions/Gamma.h"
#include "source/functions/density_function/gamma_density_function/GammaDensityFunction.h"

GammaDensityFunction::GammaDensityFunction(double alpha, double beta):
    DensityFunction(Domain<double>(), Interval(0.0, std::numeric_limits<double>::infinity())) {
    this->setAlpha(alpha);
    this->setBeta(beta);
}

void GammaDensityFunction::setAlpha(double alpha) {
    if (alpha <= 0.0) throw std::logic_error("Gamma shape (alpha) must be greater than 0");
    this->alpha = alpha;
}

void GammaDensityFunction::setBeta(double beta) {
    if (beta <= 0.0) throw std::logic_error("Gamma rate (beta) must be greater than 0");
    this->beta = beta;
}

double GammaDensityFunction::operator () (const double& input) const {
    if (input < 0.0) return 0.0;
    if (input == 0.0) {
        if (this->alpha < 1.0) return std::numeric_limits<double>::infinity();
        if (this->alpha == 1.0) return this->beta;
        return 0.0;
    }

    const double logResult = this->alpha * std::log(this->beta)
        - SpecialFunctions::logGamma(this->alpha)
        + (this->alpha - 1.0) * std::log(input)
        - this->beta * input;
    return std::exp(logResult);
}

Function<double, double>* GammaDensityFunction::clone() const {
    return new GammaDensityFunction(*this);
}
