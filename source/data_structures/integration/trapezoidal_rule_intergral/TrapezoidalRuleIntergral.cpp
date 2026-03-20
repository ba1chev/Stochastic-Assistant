#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include <cmath>
#include <stdexcept>

TrapezoidalRuleIntergral::TrapezoidalRuleIntergral(const Function<double, double>* function, size_t countOfNonSubIntervals):
    Integral(function, IntegrationApproximaton::TrapezoidalRule) {
        this->setCountOfNonSubIntervals(countOfNonSubIntervals);
}

void TrapezoidalRuleIntergral::setCountOfNonSubIntervals(size_t countOfNonSubIntervals) {
    if (countOfNonSubIntervals == 0) throw std::logic_error("Trapezoidal rule requires at least one sub-interval");
    this->countOfNonSubIntervals = countOfNonSubIntervals;
}

Integral* TrapezoidalRuleIntergral::clone() const {
    return new TrapezoidalRuleIntergral(*this);
}

double TrapezoidalRuleIntergral::intergrate(double lower, double upper) const {
    if (lower >= upper) return 0.0;
    const double h = (upper - lower) / static_cast<double>(this->countOfNonSubIntervals);
    double currentSum = 0.0;

    for (size_t i = 1; i < this->countOfNonSubIntervals; i++) {
        const double x_i = lower + i * h;
        currentSum += this->function->operator()(x_i);
    }
    
    return (h / 2.0) * (this->function->operator()(lower) + this->function->operator()(upper) + 2.0 * currentSum);
}
