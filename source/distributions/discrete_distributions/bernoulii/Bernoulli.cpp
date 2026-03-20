#include "source/distributions/discrete_distributions/bernoulii/Bernoulli.h"

Bernoulli::Bernoulli(Success success): 
    BernoulliSchemeRandomVariable(success, DiscreteRandomVariableType::Bernoulii) {}

double Bernoulli::calculateProbability(const bool& number) const {
    if (number == 1) return this->getSuccessRate();
    else if (number == 0) return this->getFailureRate();
    else throw std::logic_error("Boolean must be 0 or 1");
}

double Bernoulli::getExpectation() const {
    return this->getSuccessRate();
}

double Bernoulli::getVariance() const {
    return this->getFailureRate() * this->getSuccessRate();
}

BernoulliSchemeRandomVariable<bool>* Bernoulli::clone() const {
    return new Bernoulli(*this);
}
