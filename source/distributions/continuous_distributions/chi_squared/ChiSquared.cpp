#include <cmath>
#include <limits>
#include <algorithm>

#include "source/Constants.h"
#include "source/data_structures/integration/Integral.h"
#include "source/distributions/continuous_distributions/chi_squared/ChiSquared.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"

ChiSquared::ChiSquared(uint32_t degreesOfFreedom): ContinuousRandomVariable([&]() {
    Vector<double> params; params.push_back(static_cast<double>(degreesOfFreedom));
    return params;
}(), ContinuousRandomVariableType::ChiSquared) {}

double ChiSquared::calculateProbability(const Interval& interval) const {
    const double mean = this->getExpectation();
    const double stdDev = std::sqrt(this->getVariance());

    double left = std::max(0.0, interval.getLeftComponent());
    double right = interval.getRightComponent();

    const double effectiveRight = (std::isinf(right) && right > 0) ? mean + 10.0 * stdDev : right;
    if (left > effectiveRight) return 0.0;

    Integral* integral = new TrapezoidalRuleIntegral(this->getDensityFunction(), COUNT_OF_SUB_INTERVALS);
    double result = integral->integrate(left, effectiveRight);

    delete integral;
    integral = nullptr;
    return result;
}

double ChiSquared::getExpectation() const {
    return this->getParameters()[0];
}

double ChiSquared::getVariance() const {
    return 2.0 * this->getParameters()[0];
}

RandomVariable<Interval>* ChiSquared::clone() const {
    return new ChiSquared(*this);
}
