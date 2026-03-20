#include "source/distributions/continuous_distributions/uniform/Uniform.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/data_structures/integration/Integral.h"
#include "source/data_structures/combinatorics/k_selection/KSelection.h"
#include "source/Constants.h"

Uniform::Uniform(double lower, double upper): ContinuousRandomVariable([&]() {
    Vector<double> params;
    params.push_back(lower);
    params.push_back(upper);
    return params;
}(), ContinuousRandomVariableType::Uniform) {}

double Uniform::calculateProbability(const Interval& interval) const {
    if (interval.getLeftComponent() > interval.getRightComponent()) return 0.0;

    Integral* integral = new TrapezoidalRuleIntegral(this->getDensityFunction(), COUNT_OF_SUB_INTERVALS);
    double result = integral->integrate(interval.getLeftComponent(), interval.getRightComponent());

    delete integral;
    integral = nullptr;
    return result;
}

double Uniform::getExpectation() const {
    return (this->getParameters()[0] + this->getParameters()[1]) / 2;
}

double Uniform::getVariance() const {
    return (std::pow(this->getParameters()[1] - this->getParameters()[0], 2)) / 12;
}

RandomVariable<Interval>* Uniform::clone() const{
    return new Uniform(*this);
}