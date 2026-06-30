#pragma once

#include <cstdint>

#include "source/distributions/discrete_distributions/bernoulii/Bernoulli.h"
#include "source/data_structures/heterogeneous_container/HeterogeneousContainer.hpp"
#include "source/distributions/discrete_distributions/BernoulliSchemeRandomVariable.hpp"


class Binomial: public BernoulliSchemeRandomVariable<uint32_t> {
private:
    uint32_t countOfExperiments = 0;
    void setCountOfExperiments(uint32_t countOfExperiments);

public:
    Binomial(uint32_t countOfExperiments, Success success);
    Binomial(const HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>>& containerOfBernoullis);
    uint32_t getCountOfExperiments() const;

    double calculateProbability(const uint32_t& number) const override;
    double getExpectation() const override;
    double getVariance() const override;
    BernoulliSchemeRandomVariable<uint32_t>* clone() const override;
};