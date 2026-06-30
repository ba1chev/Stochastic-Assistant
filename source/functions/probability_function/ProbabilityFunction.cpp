#include <stdexcept>

#include "ProbabilityFunction.h"

Range<double> probabilityFunctionRange() {
    return Interval(0.0, 1.0);
}

ProbabilityFunction::ProbabilityFunction(const SigmaAlgebra* sigmaAlgebra, const Vector<Pair<Event, double>>& probabilities): 
    Function<Event, double>(sigmaAlgebra ? sigmaAlgebra->getContainerOfEvents() : Domain<Event>(), probabilityFunctionRange()) {
        this->setSigmaAlgebraPointer(sigmaAlgebra);
        this->probabilities = probabilities;
}

ProbabilityFunction::ProbabilityFunction(const SigmaAlgebra* sigmaAlgebra, ProbabilityFunctionFilter filter): 
    Function<Event, double>(sigmaAlgebra ? sigmaAlgebra->getContainerOfEvents() : Domain<Event>(), probabilityFunctionRange()) {
    this->setSigmaAlgebraPointer(sigmaAlgebra);
    this->fillProbabilities(filter);
}

const SigmaAlgebra* ProbabilityFunction::getSigmaAlgebraPointer() const {
    return this->sigmaAlgebra;
}

const Vector<Pair<Event, double>>& ProbabilityFunction::getProbabilities() const {
    return this->probabilities;
}

double ProbabilityFunction::operator () (const Event& input) const {
    for (size_t i = 0; i < this->probabilities.getSize(); i++) {
        if (this->probabilities[i].getFirstComponent() == input) {
            return this->probabilities[i].getSecondComponent();
        }
    }
    throw std::logic_error("Event is not part of the sigma-algebra domain");
}

Function<Event, double>* ProbabilityFunction::clone() const {
    return new ProbabilityFunction(*this);
}

void ProbabilityFunction::setSigmaAlgebraPointer(const SigmaAlgebra* sigmaAlgebra) {
    if (sigmaAlgebra == nullptr) {
        throw std::logic_error("SigmaAlgebra pointer is null");
    }
    this->sigmaAlgebra = sigmaAlgebra;
}

void ProbabilityFunction::fillProbabilities(ProbabilityFunctionFilter filter) {
    if (filter == nullptr) {
        throw std::logic_error("ProbabilityFunctionFilter is nullptr");
    }

    const Vector<Event>& events = sigmaAlgebra->getContainerOfEvents();
    for (size_t i = 0; i < events.getSize(); i++) {
        double value = filter(events[i]);
        if (value < 0.0 || value > 1.0) {
            throw std::logic_error("Filter returned probability outside [0, 1]");
        }
        this->probabilities.push_back(Pair<Event, double>(events[i], value));
    }
}
