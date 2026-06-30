#include <cmath>
#include <limits>

#include "source/Constants.h"
#include "source/data_structures/integration/Integral.h"
#include "source/distributions/continuous_distributions/exponential/Exponential.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/functions/density_function/exponential_density_function/ExponentialDensityFunction.h"

Exponential::Exponential(double lambda): ContinuousRandomVariable([&]() {
    Vector<double> params; params.push_back(lambda);
    return params;
}(), ContinuousRandomVariableType::Exponential) {}

double Exponential::calculateProbability(const Interval& interval) const {
    const double lambda = this->getParameters()[0];
    double left = std::max(0.0, interval.getLeftComponent());
    double right = interval.getRightComponent();

    const double effectiveRight = (std::isinf(right) && right > 0) ? EXPONENTIAL_CUTOFF_MULTIPLIER / lambda : right;
    if (left > effectiveRight) return 0.0;

    Integral* integral = new TrapezoidalRuleIntegral(this->getDensityFunction(), COUNT_OF_SUB_INTERVALS);
    double result = integral->integrate(left, effectiveRight);

    delete integral;
    integral = nullptr;
    return result;
}

double Exponential::getExpectation() const {
    return 1 / this->getParameters()[0];
}

double Exponential::getVariance() const {
    return 1 / std::pow(this->getParameters()[0], 2);
}

RandomVariable<Interval>* Exponential::clone() const {
    return new Exponential(*this);
}
