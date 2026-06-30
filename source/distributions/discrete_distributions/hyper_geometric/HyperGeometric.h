#pragma once

#include <cstdint>

#include "source/distributions/discrete_distributions/DiscreteRandomVariable.hpp"


class HyperGeometric: public DiscreteRandomVariable<uint32_t> {
private:
    uint32_t countOfAllObjects = 0;
    uint32_t countOfMarkedObjects = 0;
    uint32_t drawnObjects = 0;

    void setCountOfAllObjects(uint32_t countOfAllObjects);
    void setCountOfMarkedObjects(uint32_t countOfMarkedObjects);
    void setDrawnObjects(uint32_t drawnObjects);

public:
    HyperGeometric(uint32_t countOfAllObjects, uint32_t countOfMarkedObjects, uint32_t drawnObjects);

    double calculateProbability(const uint32_t& number) const override;
    double getExpectation() const override;
    double getVariance() const override;
    DiscreteRandomVariable<uint32_t>* clone() const override;
};