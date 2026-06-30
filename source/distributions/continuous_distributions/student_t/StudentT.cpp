#include <cmath>
#include <limits>
#include <stdexcept>

#include "source/Constants.h"
#include "source/data_structures/integration/Integral.h"
#include "source/distributions/continuous_distributions/student_t/StudentT.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"

StudentT::StudentT(uint32_t degreesOfFreedom): ContinuousRandomVariable([&]() {
    Vector<double> params; params.push_back(static_cast<double>(degreesOfFreedom));
    return params;
}(), ContinuousRandomVariableType::StudentT) {}

double StudentT::calculateProbability(const Interval& interval) const {
    const double n = this->getParameters()[0];
    const double tailBound = (n > 2.0) ? 10.0 * std::sqrt(n / (n - 2.0)) : 50.0;

    double left = interval.getLeftComponent();
    double right = interval.getRightComponent();

    const double effectiveLeft = (std::isinf(left) && left < 0) ? -tailBound : left;
    const double effectiveRight = (std::isinf(right) && right > 0) ? tailBound : right;
    if (effectiveLeft > effectiveRight) return 0.0;

    Integral* integral = new TrapezoidalRuleIntegral(this->getDensityFunction(), COUNT_OF_SUB_INTERVALS);
    double result = integral->integrate(effectiveLeft, effectiveRight);

    delete integral;
    integral = nullptr;
    return result;
}

double StudentT::getExpectation() const {
    const double n = this->getParameters()[0];
    if (n <= 1.0) throw std::logic_error("Student's t expectation undefined for n <= 1");
    return 0.0;
}

double StudentT::getVariance() const {
    const double n = this->getParameters()[0];
    if (n <= 2.0) throw std::logic_error("Student's t variance undefined for n <= 2");
    return n / (n - 2.0);
}

RandomVariable<Interval>* StudentT::clone() const {
    return new StudentT(*this);
}
