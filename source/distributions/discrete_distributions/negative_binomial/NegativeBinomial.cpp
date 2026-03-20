#include "source/distributions/discrete_distributions/negative_binomial/NegativeBinomial.h"
#include "source/data_structures/combinatorics/k_selection/KSelection.h"

NegativeBinomial::NegativeBinomial(uint32_t successesTarget, Success success): 
    BernoulliSchemeRandomVariable(success, DiscreteRandomVariableType::NegativeBinomial) {
    this->setCountOfExperiments(successesTarget);
}

NegativeBinomial::NegativeBinomial(const HeterogeneousContainer<BernoulliSchemeRandomVariable<uint32_t>>& countainerOfGeometrics)
    : BernoulliSchemeRandomVariable(countainerOfGeometrics.getSize() > 0 ? countainerOfGeometrics[0]->getSuccessRate() : 0.0, DiscreteRandomVariableType::NegativeBinomial) {
    if (countainerOfGeometrics.getSize() == 0) {
        throw std::logic_error("Negative Binomial requires at least one geometric trial");
    }

    const double probabilityForSuccess = countainerOfGeometrics[0]->getSuccessRate();
    for (size_t i = 1; i < countainerOfGeometrics.getSize(); i++) {
        if (countainerOfGeometrics[i]->getSuccessRate() != probabilityForSuccess) {
            throw std::logic_error("All geometric trials must have identical success rate");
        }
    }

    this->setCountOfExperiments(countainerOfGeometrics.getSize());
}

uint32_t NegativeBinomial::getCountOfSuccessesTarget() const {
    return this->successesTarget;
}

void NegativeBinomial::setCountOfExperiments(uint32_t successesTarget) {
    if (successesTarget == 0) throw std::logic_error("NegativeBinomial requires at least one target success");
    this->successesTarget = successesTarget;
}

double NegativeBinomial::calculateProbability(const uint32_t& number) const {
    uint32_t r = this->successesTarget;
    uint64_t combinations = HelperFunctions::binomial(number + r - 1, r - 1);
    return combinations * std::pow(this->getSuccessRate(), r) * std::pow(this->getFailureRate(), number);
}

double NegativeBinomial::getExpectation() const {
    return this->getCountOfSuccessesTarget() * this->getFailureRate() / this->getSuccessRate();
}

double NegativeBinomial::getVariance() const {
    return this->getCountOfSuccessesTarget() * this->getFailureRate() / std::pow(this->getSuccessRate(), 2);
}

BernoulliSchemeRandomVariable<uint32_t>* NegativeBinomial::clone() const {
    return new NegativeBinomial(*this);
}
