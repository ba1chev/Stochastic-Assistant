#pragma once
#include <cmath>
#include <stdexcept>

#include "source/Constants.h"
#include "source/distributions/RandomVariable.hpp"
#include "source/data_structures/vector/Vector.hpp"
#include "source/data_structures/interval/Interval.h"
#include "source/distributions/joint_distributions/JointDistribution.hpp"

using JointPDFFunctor = double (*)(const Vector<double>&);


class JointContinuousDistribution: public JointDistribution<RandomVariable<Interval>> {
private:
    Vector<Interval> supports;
    JointPDFFunctor pdfFunctor = nullptr;
    size_t stepsPerDimension = 64;

    void validateIndex(size_t index) const;
    void requireFunctor() const;
    double tensorTrapezoidal(const Vector<Interval>& region, size_t depth, Vector<double>& point) const;
    double marginalDensityAt(size_t variableIndex, double value) const;
    double meanOf(size_t variableIndex) const;

public:
    JointContinuousDistribution(const HeterogeneousContainer<RandomVariable<Interval>>& marginals, const Vector<Interval>& supports);
    JointContinuousDistribution(const HeterogeneousContainer<RandomVariable<Interval>>& marginals, const Vector<Interval>& supports, JointPDFFunctor pdfFunctor);

    void setStepsPerDimension(size_t steps);

    double jointProbability(const Vector<Interval>& region) const;
    double marginalProbability(size_t variableIndex, const Interval& region) const;
    double covariance(size_t firstIndex, size_t secondIndex) const;
    double correlation(size_t firstIndex, size_t secondIndex) const;
    bool areIndependent() const override;

    const Vector<Interval>& getSupports() const;
};

inline JointContinuousDistribution::JointContinuousDistribution(
    const HeterogeneousContainer<RandomVariable<Interval>>& marginals, const Vector<Interval>& supports):
    JointDistribution<RandomVariable<Interval>>(JointDistributionMode::Independent) {
    if (marginals.getSize() == 0) throw std::logic_error("JointContinuousDistribution requires at least one marginal");
    if (supports.getSize() != marginals.getSize()) throw std::logic_error("Supports size must match marginals size");
    this->marginals = marginals;
    this->supports = supports;
}

inline JointContinuousDistribution::JointContinuousDistribution(
    const HeterogeneousContainer<RandomVariable<Interval>>& marginals, const Vector<Interval>& supports,
    JointPDFFunctor pdfFunctor):
    JointDistribution<RandomVariable<Interval>>(JointDistributionMode::FunctorGeneral) {
    if (marginals.getSize() == 0) throw std::logic_error("JointContinuousDistribution requires at least one marginal");
    if (supports.getSize() != marginals.getSize()) throw std::logic_error("Supports size must match marginals size");
    if (pdfFunctor == nullptr) throw std::logic_error("Joint PDF functor must not be null");
    this->marginals = marginals;
    this->supports = supports;
    this->pdfFunctor = pdfFunctor;
}

inline void JointContinuousDistribution::setStepsPerDimension(size_t steps) {
    if (steps < 2) throw std::logic_error("stepsPerDimension must be at least 2");
    this->stepsPerDimension = steps;
}

inline void JointContinuousDistribution::validateIndex(size_t index) const {
    if (index >= this->getDimension()) throw std::out_of_range("Variable index is out of range");
}

inline void JointContinuousDistribution::requireFunctor() const {
    if (this->mode != JointDistributionMode::FunctorGeneral || this->pdfFunctor == nullptr) {
        throw std::logic_error("Operation requires FunctorGeneral mode with a joint PDF");
    }
}

inline double JointContinuousDistribution::tensorTrapezoidal(const Vector<Interval>& region, size_t depth,
    Vector<double>& point) const {
    const size_t dim = this->getDimension();
    const double left = region[depth].getLeftComponent();
    const double right = region[depth].getRightComponent();
    if (right <= left) return 0.0;

    const size_t n = this->stepsPerDimension;
    const double h = (right - left) / static_cast<double>(n);

    double sum = 0.0;
    for (size_t i = 0; i <= n; i++) {
        point[depth] = left + static_cast<double>(i) * h;
        double weight = (i == 0 || i == n) ? 0.5 : 1.0;

        double value;
        if (depth + 1 == dim) value = this->pdfFunctor(point);
        else value = this->tensorTrapezoidal(region, depth + 1, point);

        sum += weight * value;
    }
    return sum * h;
}

inline double JointContinuousDistribution::jointProbability(const Vector<Interval>& region) const {
    if (region.getSize() != this->getDimension()) {
        throw std::logic_error("Region size must match joint dimension");
    }

    if (this->mode == JointDistributionMode::Independent) {
        double product = 1.0;
        for (size_t i = 0; i < region.getSize(); i++) {
            product *= this->marginals[i]->calculateProbability(region[i]);
        }
        return product;
    }

    this->requireFunctor();
    Vector<double> point;
    for (size_t i = 0; i < this->getDimension(); i++) point.push_back(0.0);
    return this->tensorTrapezoidal(region, 0, point);
}

inline double JointContinuousDistribution::marginalProbability(size_t variableIndex, const Interval& region) const {
    this->validateIndex(variableIndex);

    if (this->mode == JointDistributionMode::Independent) {
        return this->marginals[variableIndex]->calculateProbability(region);
    }

    Vector<Interval> fullRegion;
    for (size_t i = 0; i < this->getDimension(); i++) {
        if (i == variableIndex) fullRegion.push_back(region);
        else fullRegion.push_back(this->supports[i]);
    }
    return this->jointProbability(fullRegion);
}

inline double JointContinuousDistribution::marginalDensityAt(size_t variableIndex, double value) const {
    this->requireFunctor();
    const size_t dim = this->getDimension();

    Vector<double> point;
    for (size_t i = 0; i < dim; i++) point.push_back(0.0);
    point[variableIndex] = value;

    if (dim == 1) return this->pdfFunctor(point);

    Vector<Interval> region;
    for (size_t i = 0; i < dim; i++) {
        if (i == variableIndex) region.push_back(Interval(value, value));
        else region.push_back(this->supports[i]);
    }

    Vector<size_t> indices;
    for (size_t i = 0; i < dim; i++) indices.push_back(0);

    const size_t n = this->stepsPerDimension;
    Vector<double> lefts;
    Vector<double> hs;
    for (size_t i = 0; i < dim; i++) {
        double l = region[i].getLeftComponent();
        double r = region[i].getRightComponent();
        lefts.push_back(l);
        hs.push_back(i == variableIndex ? 0.0 : (r - l) / static_cast<double>(n));
    }

    double sum = 0.0;
    while (true) {
        double weight = 1.0;
        for (size_t i = 0; i < dim; i++) {
            if (i == variableIndex) { point[i] = value; continue; }
            point[i] = lefts[i] + static_cast<double>(indices[i]) * hs[i];
            if (indices[i] == 0 || indices[i] == n) weight *= 0.5;
        }
        sum += weight * this->pdfFunctor(point);

        size_t k = 0;
        while (k < dim) {
            if (k == variableIndex) { k += 1; continue; }
            indices[k] += 1;
            if (indices[k] <= n) break;
            indices[k] = 0;
            k += 1;
        }
        if (k == dim) break;
    }

    double volumeWeight = 1.0;
    for (size_t i = 0; i < dim; i++) if (i != variableIndex) volumeWeight *= hs[i];
    return sum * volumeWeight;
}

inline double JointContinuousDistribution::meanOf(size_t variableIndex) const {
    return this->marginals[variableIndex]->getExpectation();
}

inline double JointContinuousDistribution::covariance(size_t firstIndex, size_t secondIndex) const {
    this->validateIndex(firstIndex);
    this->validateIndex(secondIndex);

    if (this->mode == JointDistributionMode::Independent) return 0.0;
    this->requireFunctor();

    double meanFirst = this->meanOf(firstIndex);
    double meanSecond = this->meanOf(secondIndex);

    const size_t dim = this->getDimension();
    Vector<double> point;
    for (size_t i = 0; i < dim; i++) point.push_back(0.0);

    Vector<size_t> indices;
    for (size_t i = 0; i < dim; i++) indices.push_back(0);

    const size_t n = this->stepsPerDimension;
    Vector<double> lefts;
    Vector<double> hs;
    for (size_t i = 0; i < dim; i++) {
        double l = this->supports[i].getLeftComponent();
        double r = this->supports[i].getRightComponent();
        lefts.push_back(l);
        hs.push_back((r - l) / static_cast<double>(n));
    }

    double volumeWeight = 1.0;
    for (size_t i = 0; i < dim; i++) volumeWeight *= hs[i];

    double meanProduct = 0.0;
    while (true) {
        double weight = 1.0;
        for (size_t i = 0; i < dim; i++) {
            point[i] = lefts[i] + static_cast<double>(indices[i]) * hs[i];
            if (indices[i] == 0 || indices[i] == n) weight *= 0.5;
        }
        double density = this->pdfFunctor(point);
        meanProduct += weight * point[firstIndex] * point[secondIndex] * density;

        size_t k = 0;
        while (k < dim) {
            indices[k] += 1;
            if (indices[k] <= n) break;
            indices[k] = 0;
            k += 1;
        }
        if (k == dim) break;
    }
    meanProduct *= volumeWeight;
    return meanProduct - meanFirst * meanSecond;
}

inline double JointContinuousDistribution::correlation(size_t firstIndex, size_t secondIndex) const {
    double cov = this->covariance(firstIndex, secondIndex);
    double varFirst = this->marginals[firstIndex]->getVariance();
    double varSecond = this->marginals[secondIndex]->getVariance();
    if (varFirst <= 0.0 || varSecond <= 0.0) throw std::logic_error("Correlation undefined: variance is zero");
    return cov / std::sqrt(varFirst * varSecond);
}

inline bool JointContinuousDistribution::areIndependent() const {
    if (this->mode == JointDistributionMode::Independent) return true;
    this->requireFunctor();

    const size_t dim = this->getDimension();
    const size_t samplesPerDim = 4;

    Vector<double> point;
    for (size_t i = 0; i < dim; i++) point.push_back(0.0);
    Vector<size_t> indices;
    for (size_t i = 0; i < dim; i++) indices.push_back(0);

    while (true) {
        for (size_t i = 0; i < dim; i++) {
            const Interval& sup = this->supports[i];
            const double l = sup.getLeftComponent();
            const double r = sup.getRightComponent();
            point[i] = l + (r - l) * (static_cast<double>(indices[i]) + 1.0) /
                static_cast<double>(samplesPerDim + 1);
        }

        double productOfMarginalDensities = 1.0;
        for (size_t i = 0; i < dim; i++) {
            productOfMarginalDensities *= this->marginalDensityAt(i, point[i]);
        }
        double joint = this->pdfFunctor(point);
        if (std::fabs(productOfMarginalDensities - joint) > 1e-3) return false;

        size_t k = 0;
        while (k < dim) {
            indices[k] += 1;
            if (indices[k] < samplesPerDim) break;
            indices[k] = 0;
            k += 1;
        }
        if (k == dim) break;
    }
    return true;
}

inline const Vector<Interval>& JointContinuousDistribution::getSupports() const {
    return this->supports;
}
