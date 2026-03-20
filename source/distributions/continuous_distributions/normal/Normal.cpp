#include "source/distributions/continuous_distributions/normal/Normal.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/data_structures/integration/Integral.h"
#include "source/data_structures/vector/Vector.hpp"
#include "source/functions/density_function/normal_density_function/NormalDensityFunction.h"
#include "source/Constants.h"
#include <cmath>
#include <limits>

Normal::Normal(double expectation, double sigmaSquared): ContinuousRandomVariable([&]() {
    Vector<double> params;
    params.push_back(expectation);
    params.push_back(sigmaSquared);
    return params;
}(), ContinuousRandomVariableType::Normal) {}

double Normal::calculateProbability(const Interval& interval) const {
    const double mu = this->getExpectation();
    const double sigma = std::sqrt(this->getVariance());
    double left = interval.getLeftComponent();
    double right = interval.getRightComponent();

    const double effectiveLeft = (std::isinf(left) && left < 0) ? mu - 10 * sigma : left;
    const double effectiveRight = (std::isinf(right) && right > 0) ? mu + 10 * sigma : right;
    if (effectiveLeft > effectiveRight) return 0.0;

    Integral* integral = new TrapezoidalRuleIntergral(this->getDensityFunction(), COUNT_OF_SUB_INTERVALS);
    double result = integral->intergrate(effectiveLeft, effectiveRight);

    delete integral;
    integral = nullptr;
    return result;
}

double Normal::getExpectation() const {
    return this->getParameters()[0];
}

double Normal::getVariance() const {
    return this->getParameters()[1];
}

RandomVariable<Interval>* Normal::clone() const {
    return new Normal(*this);
}
