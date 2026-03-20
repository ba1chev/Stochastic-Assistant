#pragma once

#include "source/distributions/RandomVariable.hpp"
#include "source/distributions/discrete_distributions/DiscreteRandomVariableType.h"


template <class T>
class DiscreteRandomVariable: public RandomVariable<T> {
protected:
    DiscreteRandomVariableType type = DiscreteRandomVariableType::None;
    void setType(DiscreteRandomVariableType type);

public:
    DiscreteRandomVariable(DiscreteRandomVariableType type);
    RandomVariableType getOverallType() const override;
};

template <class T>
RandomVariableType DiscreteRandomVariable<T>::getOverallType() const {
    return RandomVariableType::Discrete;
}

template <class T>
DiscreteRandomVariable<T>::DiscreteRandomVariable(DiscreteRandomVariableType type) {
    this->setType(type);
}

template <class T>
void DiscreteRandomVariable<T>::setType(DiscreteRandomVariableType type) {
    if (type == DiscreteRandomVariableType::None) throw std::runtime_error("Type must be different from None");
    this->type = type;
}