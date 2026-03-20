#include "source/distributions/discrete_distributions/binomial/Binomial.h"
#include "source/data_structures/combinatorics/k_selection/KSelection.h"

Binomial::Binomial(uint32_t countOfExperiments, Success success): 
    BernoulliSchemeRandomVariable(success, DiscreteRandomVariableType::Binomial) {
    this->setCountOfExperiments(countOfExperiments);
}

Binomial::Binomial(const HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>>& containerOfBernoullis): 
    BernoulliSchemeRandomVariable(containerOfBernoullis.getSize() > 0 ? containerOfBernoullis[0]->getSuccessRate() : 0.0, DiscreteRandomVariableType::Binomial) {
        if (containerOfBernoullis.getSize() == 0) {
            throw std::logic_error("Binomial requires at least one Bernoulli trial");
        }

        const double probabilityForSuccess = containerOfBernoullis[0]->getSuccessRate();
        for (size_t i = 1; i < containerOfBernoullis.getSize(); i++) {
            if (containerOfBernoullis[i]->getSuccessRate() != probabilityForSuccess) {
                throw std::logic_error("All Bernoulli trials must have identical success rate");
            }
        }

        this->setCountOfExperiments(containerOfBernoullis.getSize());
}

uint32_t Binomial::getCountOfExperiments() const {
    return this->countOfExperiments;
}

void Binomial::setCountOfExperiments(uint32_t countOfExperiments) {
    if (countOfExperiments == 0) throw std::logic_error("Binomial requires at least one experiment");
    this->countOfExperiments = countOfExperiments;
}

double Binomial::calculateProbability(const uint32_t& number) const {
    if (number > this->countOfExperiments) return 0.0;
    return KSelection(KSelectionPattern::Combination, this->countOfExperiments, number).generateCount() * 
        std::pow(this->getSuccessRate(), number) * 
        std::pow(this->getFailureRate(), this->countOfExperiments - number);
}

double Binomial::getExpectation() const {
    return this->getSuccessRate() * this->countOfExperiments;
}

double Binomial::getVariance() const {
    return this->getSuccessRate() * this->getFailureRate() * this->countOfExperiments;
}

BernoulliSchemeRandomVariable<uint32_t>* Binomial::clone() const {
    return new Binomial(*this);
}