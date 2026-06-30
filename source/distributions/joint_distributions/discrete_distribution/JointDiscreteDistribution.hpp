#pragma once
#include <cmath>
#include <stdexcept>

#include "source/Constants.h"
#include "source/data_structures/pair/Pair.hpp"
#include "source/distributions/RandomVariable.hpp"
#include "source/data_structures/vector/Vector.hpp"
#include "source/distributions/joint_distributions/JointDistribution.hpp"

template <class T> using JointPMFFunctor = double (*)(const Vector<T>&);


template <class T>
class JointDiscreteDistribution: public JointDistribution<RandomVariable<T>> {
private:
    Vector<Pair<Vector<T>, double>> table;
    JointPMFFunctor<T> pmfFunctor = nullptr;
    Vector<Vector<T>> supports;

    void validateProbability(double probability) const;
    void validateIndex(size_t index) const;
    void enumerateSupports(size_t fixedIndex, const Vector<T>& fixedValues, Vector<Vector<T>>& output) const;

public:
    JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals, const Vector<Vector<T>>& supports);
    JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals, const Vector<Vector<T>>& supports, const Vector<Pair<Vector<T>, double>>& table);
    JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals, const Vector<Vector<T>>& supports, JointPMFFunctor<T> pmfFunctor);

    double jointProbability(const Vector<T>& point) const;
    Vector<Pair<T, double>> marginal(size_t variableIndex) const;
    Vector<Pair<T, double>> conditional(size_t targetIndex, size_t givenIndex, const T& givenValue) const;
    double covariance(size_t firstIndex, size_t secondIndex) const;
    double correlation(size_t firstIndex, size_t secondIndex) const;
    bool areIndependent() const override;

    const Vector<Pair<Vector<T>, double>>& getTable() const;
    const Vector<Vector<T>>& getSupports() const;
};

template <class T>
JointDiscreteDistribution<T>::JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals,
    const Vector<Vector<T>>& supports):
    JointDistribution<RandomVariable<T>>(JointDistributionMode::Independent) {
    if (marginals.getSize() == 0) throw std::logic_error(ERR_JOINT_REQUIRES_MARGINAL);
    if (supports.getSize() != marginals.getSize()) throw std::logic_error(ERR_SUPPORTS_SIZE_MISMATCH);
    this->marginals = marginals;
    this->supports = supports;
}

template <class T>
JointDiscreteDistribution<T>::JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals,
    const Vector<Vector<T>>& supports, const Vector<Pair<Vector<T>, double>>& table):
    JointDistribution<RandomVariable<T>>(JointDistributionMode::TableGeneral) {
    if (marginals.getSize() == 0) throw std::logic_error(ERR_JOINT_REQUIRES_MARGINAL);
    if (supports.getSize() != marginals.getSize()) throw std::logic_error(ERR_SUPPORTS_SIZE_MISMATCH);
    if (table.getSize() == 0) throw std::logic_error("Joint PMF table must be non-empty");

    double total = 0.0;
    for (size_t i = 0; i < table.getSize(); i++) {
        if (table[i].getFirstComponent().getSize() != marginals.getSize()) {
            throw std::logic_error("Table tuple size must match marginals size");
        }
        this->validateProbability(table[i].getSecondComponent());
        total += table[i].getSecondComponent();
    }
    if (std::fabs(total - 1.0) > JOINT_DISCRETE_PMF_SUM_TOLERANCE) throw std::logic_error("Joint PMF table probabilities must sum to 1");

    this->marginals = marginals;
    this->supports = supports;
    this->table = table;
}

template <class T>
JointDiscreteDistribution<T>::JointDiscreteDistribution(const HeterogeneousContainer<RandomVariable<T>>& marginals,
    const Vector<Vector<T>>& supports, JointPMFFunctor<T> pmfFunctor):
    JointDistribution<RandomVariable<T>>(JointDistributionMode::FunctorGeneral) {
    if (marginals.getSize() == 0) throw std::logic_error(ERR_JOINT_REQUIRES_MARGINAL);
    if (supports.getSize() != marginals.getSize()) throw std::logic_error(ERR_SUPPORTS_SIZE_MISMATCH);
    if (pmfFunctor == nullptr) throw std::logic_error("Joint PMF functor must not be null");

    this->marginals = marginals;
    this->supports = supports;
    this->pmfFunctor = pmfFunctor;
}

template <class T>
void JointDiscreteDistribution<T>::validateProbability(double probability) const {
    if (probability < 0.0 || probability > 1.0) throw std::logic_error("Probability must be in [0, 1]");
}

template <class T>
void JointDiscreteDistribution<T>::validateIndex(size_t index) const {
    if (index >= this->getDimension()) throw std::out_of_range("Variable index is out of range");
}

template <class T>
double JointDiscreteDistribution<T>::jointProbability(const Vector<T>& point) const {
    if (point.getSize() != this->getDimension()) {
        throw std::logic_error("Point size must match joint dimension");
    }

    switch (this->mode) {
        case JointDistributionMode::Independent: {
            double product = 1.0;
            for (size_t i = 0; i < point.getSize(); i++) {
                product *= this->marginals[i]->calculateProbability(point[i]);
            }
            return product;
        }
        case JointDistributionMode::TableGeneral: {
            for (size_t i = 0; i < this->table.getSize(); i++) {
                const Vector<T>& key = this->table[i].getFirstComponent();
                bool match = true;
                for (size_t j = 0; j < key.getSize(); j++) {
                    if (!(key[j] == point[j])) { match = false; break; }
                }
                if (match) return this->table[i].getSecondComponent();
            }
            return 0.0;
        }
        case JointDistributionMode::FunctorGeneral: return this->pmfFunctor(point);
        default: throw std::logic_error("Unsupported joint distribution mode");
    }
}

template <class T>
void JointDiscreteDistribution<T>::enumerateSupports(size_t fixedIndex, const Vector<T>& fixedValues,
    Vector<Vector<T>>& output) const {
    const size_t dim = this->getDimension();
    Vector<size_t> indices;
    for (size_t i = 0; i < dim; i++) indices.push_back(0);

    while (true) {
        Vector<T> point;
        for (size_t i = 0; i < dim; i++) point.push_back(this->supports[i][indices[i]]);

        bool skip = false;
        for (size_t i = 0; i < fixedIndex; i++) {
            if (!(point[i] == fixedValues[i])) { skip = true; break; }
        }
        if (!skip) output.push_back(point);

        size_t k = 0;
        while (k < dim) {
            indices[k] += 1;
            if (indices[k] < this->supports[k].getSize()) break;
            indices[k] = 0;
            k += 1;
        }
        if (k == dim) break;
    }
}

template <class T>
Vector<Pair<T, double>> JointDiscreteDistribution<T>::marginal(size_t variableIndex) const {
    this->validateIndex(variableIndex);
    Vector<Pair<T, double>> result;

    const Vector<T>& support = this->supports[variableIndex];
    for (size_t i = 0; i < support.getSize(); i++) {
        const T& value = support[i];
        if (this->mode == JointDistributionMode::Independent) {
            result.push_back(Pair<T, double>(value, this->marginals[variableIndex]->calculateProbability(value)));
            continue;
        }

        Vector<Vector<T>> allPoints;
        Vector<T> empty;
        this->enumerateSupports(0, empty, allPoints);

        double sum = 0.0;
        for (size_t j = 0; j < allPoints.getSize(); j++) {
            if (allPoints[j][variableIndex] == value) {
                sum += this->jointProbability(allPoints[j]);
            }
        }
        result.push_back(Pair<T, double>(value, sum));
    }
    return result;
}

template <class T>
Vector<Pair<T, double>> JointDiscreteDistribution<T>::conditional(size_t targetIndex, size_t givenIndex,
    const T& givenValue) const {
    this->validateIndex(targetIndex);
    this->validateIndex(givenIndex);
    if (targetIndex == givenIndex) throw std::logic_error("Conditional requires distinct indices");

    Vector<Pair<T, double>> givenMarginal = this->marginal(givenIndex);
    double pGiven = 0.0;
    for (size_t i = 0; i < givenMarginal.getSize(); i++) {
        if (givenMarginal[i].getFirstComponent() == givenValue) {
            pGiven = givenMarginal[i].getSecondComponent();
            break;
        }
    }
    if (pGiven == 0.0) throw std::logic_error("Conditional undefined: P(given) == 0");

    Vector<Pair<T, double>> result;
    const Vector<T>& targetSupport = this->supports[targetIndex];
    Vector<Vector<T>> allPoints;
    Vector<T> empty;
    this->enumerateSupports(0, empty, allPoints);

    for (size_t i = 0; i < targetSupport.getSize(); i++) {
        const T& targetValue = targetSupport[i];
        double joint = 0.0;
        for (size_t j = 0; j < allPoints.getSize(); j++) {
            if (allPoints[j][targetIndex] == targetValue && allPoints[j][givenIndex] == givenValue) {
                joint += this->jointProbability(allPoints[j]);
            }
        }
        result.push_back(Pair<T, double>(targetValue, joint / pGiven));
    }
    return result;
}

template <class T>
double JointDiscreteDistribution<T>::covariance(size_t firstIndex, size_t secondIndex) const {
    this->validateIndex(firstIndex);
    this->validateIndex(secondIndex);

    Vector<Pair<T, double>> firstMarginal = this->marginal(firstIndex);
    Vector<Pair<T, double>> secondMarginal = this->marginal(secondIndex);

    double meanFirst = 0.0;
    for (size_t i = 0; i < firstMarginal.getSize(); i++) {
        meanFirst += static_cast<double>(firstMarginal[i].getFirstComponent()) * firstMarginal[i].getSecondComponent();
    }
    double meanSecond = 0.0;
    for (size_t i = 0; i < secondMarginal.getSize(); i++) {
        meanSecond += static_cast<double>(secondMarginal[i].getFirstComponent()) * secondMarginal[i].getSecondComponent();
    }

    Vector<Vector<T>> allPoints;
    Vector<T> empty;
    this->enumerateSupports(0, empty, allPoints);

    double meanProduct = 0.0;
    for (size_t i = 0; i < allPoints.getSize(); i++) {
        const Vector<T>& point = allPoints[i];
        meanProduct += static_cast<double>(point[firstIndex]) * static_cast<double>(point[secondIndex]) *
            this->jointProbability(point);
    }
    return meanProduct - meanFirst * meanSecond;
}

template <class T>
double JointDiscreteDistribution<T>::correlation(size_t firstIndex, size_t secondIndex) const {
    double cov = this->covariance(firstIndex, secondIndex);
    double varFirst = this->marginals[firstIndex]->getVariance();
    double varSecond = this->marginals[secondIndex]->getVariance();
    if (varFirst <= 0.0 || varSecond <= 0.0) throw std::logic_error(ERR_CORRELATION_ZERO_VARIANCE);
    return cov / std::sqrt(varFirst * varSecond);
}

template <class T>
bool JointDiscreteDistribution<T>::areIndependent() const {
    if (this->mode == JointDistributionMode::Independent) return true;

    Vector<Vector<T>> allPoints;
    Vector<T> empty;
    this->enumerateSupports(0, empty, allPoints);

    for (size_t i = 0; i < allPoints.getSize(); i++) {
        const Vector<T>& point = allPoints[i];
        double product = 1.0;
        for (size_t j = 0; j < this->getDimension(); j++) {
            product *= this->marginals[j]->calculateProbability(point[j]);
        }
        if (std::fabs(product - this->jointProbability(point)) > EPS) return false;
    }
    return true;
}

template <class T>
const Vector<Pair<Vector<T>, double>>& JointDiscreteDistribution<T>::getTable() const {
    return this->table;
}

template <class T>
const Vector<Vector<T>>& JointDiscreteDistribution<T>::getSupports() const {
    return this->supports;
}
