#include "source/functions/density_function/student_t_density_function/StudentTDensityFunction.h"
#include "source/data_structures/special_functions/Gamma.h"
#include "source/Constants.h"
#include <cmath>
#include <limits>
#include <stdexcept>

StudentTDensityFunction::StudentTDensityFunction(uint32_t degreesOfFreedom):
    DensityFunction(Domain<double>(),
        Interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity())) {
    this->setDegreesOfFreedom(degreesOfFreedom);
}

void StudentTDensityFunction::setDegreesOfFreedom(uint32_t degreesOfFreedom) {
    if (degreesOfFreedom == 0) throw std::logic_error("Student's t degrees of freedom must be >= 1");
    this->degreesOfFreedom = degreesOfFreedom;
}

double StudentTDensityFunction::operator () (const double& input) const {
    const double n = static_cast<double>(this->degreesOfFreedom);

    const double logNormalization = SpecialFunctions::logGamma((n + 1.0) / 2.0)
        - 0.5 * std::log(n * PI)
        - SpecialFunctions::logGamma(n / 2.0);
    const double logKernel = -((n + 1.0) / 2.0) * std::log(1.0 + (input * input) / n);
    return std::exp(logNormalization + logKernel);
}

Function<double, double>* StudentTDensityFunction::clone() const {
    return new StudentTDensityFunction(*this);
}
