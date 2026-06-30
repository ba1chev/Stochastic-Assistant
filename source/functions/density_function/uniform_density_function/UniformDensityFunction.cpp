#include <stdexcept>

#include "source/functions/density_function/uniform_density_function/UniformDensityFunction.h"

UniformDensityFunction::UniformDensityFunction(const Interval& interval): DensityFunction() {
    this->setInterval(interval);
}

void UniformDensityFunction::setInterval(const Interval& interval) {
    if (interval.getRightComponent() <= interval.getLeftComponent()) {
        throw std::logic_error("Uniform density interval must have positive length");
    }
    this->interval = interval;
}

double UniformDensityFunction::operator () (const double& input) const {
    if (input < this->interval.getLeftComponent() || input > this->interval.getRightComponent()) {
        return 0.0;
    }
    const double length = this->interval.getRightComponent() - this->interval.getLeftComponent();
    return 1.0 / length;
}

Function<double, double>* UniformDensityFunction::clone() const {
    return new UniformDensityFunction(*this);
}
