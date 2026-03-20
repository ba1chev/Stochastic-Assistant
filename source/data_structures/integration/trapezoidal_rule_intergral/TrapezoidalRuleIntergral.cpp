#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include <cmath>
#include <stdexcept>

TrapezoidalRuleIntegral::TrapezoidalRuleIntegral(const Function<double, double>* function, size_t countOfSubIntervals):
    Integral(function, IntegrationApproximation::TrapezoidalRule) {
        this->setCountOfNonSubIntervals(countOfSubIntervals);
}

void TrapezoidalRuleIntegral::setCountOfNonSubIntervals(size_t countOfSubIntervals) {
    if (countOfSubIntervals == 0) throw std::logic_error("Trapezoidal rule requires at least one sub-interval");
    this->countOfSubIntervals = countOfSubIntervals;
}

Integral* TrapezoidalRuleIntegral::clone() const {
    return new TrapezoidalRuleIntegral(*this);
}

double TrapezoidalRuleIntegral::integrate(double lower, double upper) const {
    if (lower >= upper) return 0.0;
    const double h = (upper - lower) / static_cast<double>(this->countOfSubIntervals);
    double currentSum = 0.0;

    for (size_t i = 1; i < this->countOfSubIntervals; i++) {
        const double x_i = lower + i * h;
        currentSum += this->function->operator()(x_i);
    }
    
    return (h / 2.0) * (this->function->operator()(lower) + this->function->operator()(upper) + 2.0 * currentSum);
}
