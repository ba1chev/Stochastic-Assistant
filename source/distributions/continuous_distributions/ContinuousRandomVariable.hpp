#pragma once

#include "source/distributions/RandomVariable.hpp"
#include "source/distributions/continuous_distributions/ContinuousRandomVariableType.h"
#include "source/data_structures/interval/Interval.h"
#include "source/functions/Function.hpp"
#include "source/functions/density_function/uniform_density_function/UniformDensityFunction.h"
#include "source/data_structures/vector/Vector.hpp"
#include "source/functions/density_function/normal_density_function/NormalDensityFunction.h"
#include "source/functions/density_function/exponential_density_function/ExponentialDensityFunction.h"
#include "source/functions/density_function/gamma_density_function/GammaDensityFunction.h"
#include "source/functions/density_function/chi_squared_density_function/ChiSquaredDensityFunction.h"
#include "source/functions/density_function/student_t_density_function/StudentTDensityFunction.h"


template <class T>
class ContinuousRandomVariable: public RandomVariable<Interval> {
protected:
    Vector<T> parameters;
    const Function<double, double>* densityFunction = nullptr;
    ContinuousRandomVariableType type = ContinuousRandomVariableType::None;

    void setType(ContinuousRandomVariableType type);
    void setParameters(const Vector<T>& inputParameters);
    void setDensityFunction();

    void free();
    void moveTo(ContinuousRandomVariable&& other) noexcept;
    void copyFrom(const ContinuousRandomVariable& other);

public:
    ContinuousRandomVariable(const Vector<T>& inputParams, ContinuousRandomVariableType type);
    ContinuousRandomVariable(const ContinuousRandomVariable& other);
    ContinuousRandomVariable(ContinuousRandomVariable&& other) noexcept;
    ContinuousRandomVariable& operator = (const ContinuousRandomVariable& other);
    ContinuousRandomVariable& operator = (ContinuousRandomVariable&& other) noexcept;
    ~ContinuousRandomVariable();

    const Vector<T>& getParameters() const;
    const Function<double, double>* getDensityFunction() const;
    RandomVariableType getOverallType() const override;
};

template <class T>
ContinuousRandomVariable<T>::ContinuousRandomVariable(const Vector<T>& inputParams, ContinuousRandomVariableType type) {
    this->setType(type);
    this->setParameters(inputParams);
    this->setDensityFunction();
}

template <class T>
ContinuousRandomVariable<T>::ContinuousRandomVariable(const ContinuousRandomVariable& other) {
    this->copyFrom(other);
}

template <class T>
ContinuousRandomVariable<T>::ContinuousRandomVariable(ContinuousRandomVariable&& other) noexcept {
    this->moveTo(std::move(other));
}

template <class T>
ContinuousRandomVariable<T>& ContinuousRandomVariable<T>::operator = (const ContinuousRandomVariable& other) {
    if (this != &other) {
        this->free();
        this->copyFrom(other);
    }

    return *this;
}

template <class T>
ContinuousRandomVariable<T>& ContinuousRandomVariable<T>::operator = (ContinuousRandomVariable&& other) noexcept {
    if (this != &other) {
        this->free();
        this->moveTo(std::move(other));
    }

    return *this;
}

template <class T>
ContinuousRandomVariable<T>::~ContinuousRandomVariable() {
    this->free();
}

template <class T>
void ContinuousRandomVariable<T>::free() {
    delete this->densityFunction;
    this->densityFunction = nullptr;
}

template <class T>
void ContinuousRandomVariable<T>::setType(ContinuousRandomVariableType type) {
    if (type == ContinuousRandomVariableType::None) throw std::runtime_error("Type must be different from None");
    this->type = type;
}

template <class T>
void ContinuousRandomVariable<T>::setDensityFunction() {
    switch (this->type) {
    case ContinuousRandomVariableType::Uniform: 
        this->densityFunction = new UniformDensityFunction(Interval(this->parameters[0], this->parameters[1])); break;
    case ContinuousRandomVariableType::Normal: 
        this->densityFunction = new NormalDensityFunction(this->parameters[0], this->parameters[1]); break;
    case ContinuousRandomVariableType::Exponential:
        this->densityFunction = new ExponentialDensityFunction(this->parameters[0]); break;
    case ContinuousRandomVariableType::Gamma:
        this->densityFunction = new GammaDensityFunction(this->parameters[0], this->parameters[1]); break;
    case ContinuousRandomVariableType::ChiSquared:
        this->densityFunction = new ChiSquaredDensityFunction(static_cast<uint32_t>(this->parameters[0])); break;
    case ContinuousRandomVariableType::StudentT:
        this->densityFunction = new StudentTDensityFunction(static_cast<uint32_t>(this->parameters[0])); break;
    default: throw std::runtime_error("Unsupported ContinuousRandomVariable");
    }
}

template <class T>
void ContinuousRandomVariable<T>::moveTo(ContinuousRandomVariable&& other) noexcept {
    this->parameters = other.parameters;
    this->type = other.type;
    this->densityFunction = other.densityFunction;
    
    other.densityFunction = nullptr;
    other.type = ContinuousRandomVariableType::None;
}

template <class T>
void ContinuousRandomVariable<T>::copyFrom(const ContinuousRandomVariable& other) {
    this->parameters = other.parameters;
    this->type = other.type;
    switch (this->type) {
    case ContinuousRandomVariableType::Uniform:
        this->densityFunction = new UniformDensityFunction(Interval(other.getParameters()[0], other.getParameters()[1])); break;
    case ContinuousRandomVariableType::Normal:
        this->densityFunction = new NormalDensityFunction(other.getParameters()[0], other.getParameters()[1]); break;
    case ContinuousRandomVariableType::Exponential:
        this->densityFunction = new ExponentialDensityFunction(other.getParameters()[0]); break;
    case ContinuousRandomVariableType::Gamma:
        this->densityFunction = new GammaDensityFunction(other.getParameters()[0], other.getParameters()[1]); break;
    case ContinuousRandomVariableType::ChiSquared:
        this->densityFunction = new ChiSquaredDensityFunction(static_cast<uint32_t>(other.getParameters()[0])); break;
    case ContinuousRandomVariableType::StudentT:
        this->densityFunction = new StudentTDensityFunction(static_cast<uint32_t>(other.getParameters()[0])); break;
    default:
        throw std::runtime_error("Unsupported ContinuousRandomVariable");
    }
}

template <class T>
void ContinuousRandomVariable<T>::setParameters(const Vector<T>& inputParameters) {
    if (this->type == ContinuousRandomVariableType::Uniform && (inputParameters.getSize() != 2 || inputParameters[0] >= inputParameters[1])) {
        throw std::runtime_error("Uniform distribution requires two parameters with lower < upper");
    } else if (this->type == ContinuousRandomVariableType::Normal && (inputParameters.getSize() != 2)) {
        throw std::runtime_error("Normal distribution requires two parameters");
    } else if (this->type == ContinuousRandomVariableType::Exponential && (inputParameters.getSize() != 1 || inputParameters[0] <= 0)) {
        throw std::runtime_error("Exponential distribution requires one parameter and it must be greater than 0");
    } else if (this->type == ContinuousRandomVariableType::Gamma && (inputParameters.getSize() != 2 || inputParameters[0] <= 0 || inputParameters[1] <= 0)) {
        throw std::runtime_error("Gamma distribution requires two positive parameters (shape, rate)");
    } else if (this->type == ContinuousRandomVariableType::ChiSquared && (inputParameters.getSize() != 1 || inputParameters[0] < 1)) {
        throw std::runtime_error("Chi-squared distribution requires one parameter (degrees of freedom >= 1)");
    } else if (this->type == ContinuousRandomVariableType::StudentT && (inputParameters.getSize() != 1 || inputParameters[0] < 1)) {
        throw std::runtime_error("Student's t distribution requires one parameter (degrees of freedom >= 1)");
    }
    this->parameters = inputParameters;
}

template <class T>
const Vector<T>& ContinuousRandomVariable<T>::getParameters() const {
    return this->parameters;
}

template <class T>
const Function<double, double>* ContinuousRandomVariable<T>::getDensityFunction() const {
    return this->densityFunction;
}

template <class T>
RandomVariableType ContinuousRandomVariable<T>::getOverallType() const {
    return RandomVariableType::Continuous;
}
