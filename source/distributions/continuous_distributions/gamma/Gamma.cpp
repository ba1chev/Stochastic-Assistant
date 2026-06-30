#include "source/distributions/continuous_distributions/gamma/Gamma.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/data_structures/integration/Integral.h"
#include "source/Constants.h"
#include <algorithm>
#include <cmath>
#include <limits>

Gamma::Gamma(double alpha, double beta): ContinuousRandomVariable([&]() {
    Vector<double> params;
    params.push_back(alpha);
    params.push_back(beta);
    return params;
}(), ContinuousRandomVariableType::Gamma) {}

double Gamma::calculateProbability(const Interval& interval) const {
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

double Gamma::getExpectation() const {
    return this->getParameters()[0] / this->getParameters()[1];
}

double Gamma::getVariance() const {
    return this->getParameters()[0] / std::pow(this->getParameters()[1], 2);
}

RandomVariable<Interval>* Gamma::clone() const {
    return new Gamma(*this);
}
