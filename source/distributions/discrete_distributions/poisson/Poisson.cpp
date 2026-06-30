#include <stdexcept>

#include "source/Constants.h"
#include "source/data_structures/combinatorics/factoriel/Factoriel.h"
#include "source/distributions/discrete_distributions/poisson/Poisson.h"

Poisson::Poisson(double lambda): 
    DiscreteRandomVariable(DiscreteRandomVariableType::Poisson) {
    this->setLambda(lambda);
}

void Poisson::setLambda(double lambda) {
    if (lambda <= 0.0) throw std::logic_error("Poisson lambda must be positive");
    this->lambda = lambda;
}

double Poisson::calculateProbability(const uint32_t& number) const {
    double numerator = std::pow(this->lambda, static_cast<double>(number));
    double denominator = static_cast<double>(Factorial()(number));
    double result = std::exp(-this->lambda) * (numerator / denominator);
    return result;
}

double Poisson::getExpectation() const {
    return this->lambda;
}

double Poisson::getVariance() const {
    return this->lambda;
}

RandomVariable<uint32_t>* Poisson::clone() const {
    return new Poisson(*this);
}