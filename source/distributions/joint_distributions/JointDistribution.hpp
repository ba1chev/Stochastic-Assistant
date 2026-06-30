#pragma once

#include "source/distributions/RandomVariable.hpp"
#include "source/distributions/joint_distributions/JointDistributionMode.h"
#include "source/data_structures/heterogeneous_container/HeterogeneousContainer.hpp"


template <class T>
class JointDistribution {
protected:
    HeterogeneousContainer<T> marginals;
    JointDistributionMode mode = JointDistributionMode::None;

    void setMode(JointDistributionMode mode);

public:
    JointDistribution() = default;
    explicit JointDistribution(JointDistributionMode mode);
    virtual ~JointDistribution() = default;

    void addRandomVariable(const T& randomVariable);
    void addRandomVariable(const T* randomVariable);
    void removeRandomVariable(size_t index);

    size_t getDimension() const;
    JointDistributionMode getMode() const;
    const HeterogeneousContainer<T>& getMarginals() const;

    virtual bool areIndependent() const = 0;
};

template <class T>
JointDistribution<T>::JointDistribution(JointDistributionMode mode) {
    this->setMode(mode);
}

template <class T>
void JointDistribution<T>::setMode(JointDistributionMode mode) {
    if (mode == JointDistributionMode::None) throw std::logic_error("JointDistribution mode must not be None");
    this->mode = mode;
}

template <class T>
void JointDistribution<T>::addRandomVariable(const T& randomVariable) {
    this->marginals.addElement(randomVariable);
}

template <class T>
void JointDistribution<T>::addRandomVariable(const T* randomVariable) {
    this->marginals.addElement(randomVariable);
}

template <class T>
void JointDistribution<T>::removeRandomVariable(size_t index) {
    this->marginals.removeElement(index);
}

template <class T>
size_t JointDistribution<T>::getDimension() const {
    return this->marginals.getSize();
}

template <class T>
JointDistributionMode JointDistribution<T>::getMode() const {
    return this->mode;
}

template <class T>
const HeterogeneousContainer<T>& JointDistribution<T>::getMarginals() const {
    return this->marginals;
}
