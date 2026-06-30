#include <algorithm>

#include "source/data_structures/combinatorics/k_selection/KSelection.h"
#include "source/data_structures/combinatorics/k_selection/KSelectionPattern.h"
#include "source/distributions/discrete_distributions/hyper_geometric/HyperGeometric.h"

HyperGeometric::HyperGeometric(uint32_t countOfAllObjects, uint32_t countOfMarkedObjects, uint32_t drawnObjects):
    DiscreteRandomVariable(DiscreteRandomVariableType::HyperGeometric) {
    if (countOfAllObjects >= countOfMarkedObjects && countOfAllObjects >= drawnObjects) {
        this->setCountOfAllObjects(countOfAllObjects);
        this->setCountOfMarkedObjects(countOfMarkedObjects);
        this->setDrawnObjects(drawnObjects);
    } else {
        throw std::runtime_error("Count of all objects must be >= count of marked objects and >= drawn objects");
    }
}

double HyperGeometric::calculateProbability(const uint32_t& number) const {
    uint32_t maxK = std::min(this->drawnObjects, this->countOfMarkedObjects);
    uint32_t minK = (this->drawnObjects + this->countOfMarkedObjects > this->countOfAllObjects)
        ? (this->drawnObjects + this->countOfMarkedObjects - this->countOfAllObjects) : 0;
    if (number < minK || number > maxK) return 0.0;

    double numerator = (double)KSelection(KSelectionPattern::Combination, this->countOfMarkedObjects, number).generateCount() *
        (double)KSelection(KSelectionPattern::Combination, this->countOfAllObjects - this->countOfMarkedObjects, this->drawnObjects - number).generateCount();
    double denominator = (double)KSelection(KSelectionPattern::Combination, this->countOfAllObjects, this->drawnObjects).generateCount();

    return numerator / denominator;
}

double HyperGeometric::getExpectation() const {
    return (double)(this->drawnObjects * this->countOfMarkedObjects) / this->countOfAllObjects;
}

double HyperGeometric::getVariance() const {
    if (this->countOfAllObjects <= 1) return 0.0;
    return this->getExpectation() * ((double)(this->countOfAllObjects - this->countOfMarkedObjects) / this->countOfAllObjects) *
        ((double)(this->countOfAllObjects - this->drawnObjects) / (this->countOfAllObjects - 1));
}

DiscreteRandomVariable<uint32_t>* HyperGeometric::clone() const {
    return new HyperGeometric(*this);
}

void HyperGeometric::setCountOfAllObjects(uint32_t countOfAllObjects) {
    this->countOfAllObjects = countOfAllObjects;
}

void HyperGeometric::setCountOfMarkedObjects(uint32_t countOfMarkedObjects) {
    this->countOfMarkedObjects = countOfMarkedObjects;
}

void HyperGeometric::setDrawnObjects(uint32_t drawnObjects) {
    this->drawnObjects = drawnObjects;
}