#include <cmath>
#include <limits>

#include "source/Constants.h"
#include "source/data_structures/combinatorics/k_selection/KSelection.h"
#include "source/functions/density_function/normal_density_function/NormalDensityFunction.h"

NormalDensityFunction::NormalDensityFunction(double expectation, double sigma): 
    DensityFunction(Domain<double>(), Interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity())) {
    this->setExpectation(expectation);
    this->setSigma(sigma);
}

void NormalDensityFunction::setSigma(double sigmaSquared) {
    if (sigmaSquared <= 0) throw std::logic_error("sigmaSquared parameter must be greater than zero");
    this->sigmaSquared = sigmaSquared;
}

void NormalDensityFunction::setExpectation(double expectation) {
    this->expectation = expectation;
}

double NormalDensityFunction::operator () (const double& input) const {
    const double exponent = -std::pow(input - this->expectation, 2) /
        (2 * this->sigmaSquared);

    return std::pow(EULER_NUMBER, exponent) /
        std::sqrt(2 * PI * this->sigmaSquared);
}

Function<double, double>* NormalDensityFunction::clone() const {
    return new NormalDensityFunction(*this);
}
