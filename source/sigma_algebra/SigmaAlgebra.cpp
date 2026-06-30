#include <stdexcept>

#include "SigmaAlgebra.h"
#include "sigma_algebra_factory/SigmaAlgebraFactory.h"

SigmaAlgebra::SigmaAlgebra(Omega* omega, SigmaAlgebraPattern pattern) {
    this->setOmegaRef(omega);
    this->setSigmaAlgebraPattern(pattern);

    switch (pattern) {
        case SigmaAlgebraPattern::Trivial: this->buildTrivial(); break;
        case SigmaAlgebraPattern::PowerSet: this->buildPowerSet(); break;
        default: throw std::logic_error("Unsupported sigma-algebra pattern");
    }
}

void SigmaAlgebra::setOmegaRef(Omega* omega) {
    if (omega == nullptr) throw std::logic_error("Nullptr error");
    this->omega = omega;
}

void SigmaAlgebra::setSigmaAlgebraPattern(SigmaAlgebraPattern pattern) {
    if (pattern == SigmaAlgebraPattern::None) throw std::logic_error("Not selected pattern");
    this->pattern = pattern;
}

const Vector<Event>& SigmaAlgebra::getContainerOfEvents() const {
    return this->containerOfEvents;
}

void SigmaAlgebra::addEmptyEvent() {
    Event emptyEvent;
    this->containerOfEvents.push_back(emptyEvent);
}

void SigmaAlgebra::addTheAddition() {
    size_t currentSize = this->containerOfEvents.getSize();
    for (size_t i = 0; i < currentSize; i++) {
        Event complement = makeComplement(this->containerOfEvents[i]);
        this->containerOfEvents.push_back(complement);
    }
}

void SigmaAlgebra::addTheUnionOfAddition() {
    size_t currentSize = this->containerOfEvents.getSize();
    if (currentSize > 32) return;

    for (size_t i = 0; i < currentSize; i++) {
        Event complement = makeComplement(this->containerOfEvents[i]);
        Event united = makeUnion(this->containerOfEvents[i], complement);
        this->containerOfEvents.push_back(united);
    }
}

void SigmaAlgebra::addOmega() {
    Event event;
    const Vector<ElementaryEvent>& elementaryEvents = this->omega->getElementaryEvents();
    for (size_t i = 0; i < elementaryEvents.getSize(); i++) {
        event.addElementaryEvent(elementaryEvents[i]);
    }
    this->containerOfEvents.push_back(event);
}

void SigmaAlgebra::buildTrivial() {
    this->containerOfEvents = Vector<Event>();
    this->addEmptyEvent();
    this->addOmega();
}

void SigmaAlgebra::buildPowerSet() {
    this->containerOfEvents = Vector<Event>();
    const size_t n = this->omega->getElementaryEvents().getSize();
    if (n > 20) {
        throw std::logic_error("Omega is too large to construct power set explicitly (n > 20)");
    }

    size_t total = 1 << n;
    for (size_t mask = 0; mask < total; mask++) {
        this->containerOfEvents.push_back(makeEventFromMask(mask));
    }
}

Event SigmaAlgebra::makeEventFromMask(size_t mask) const {
    Event event;
    const Vector<ElementaryEvent>& omegaEvents = this->omega->getElementaryEvents();

    for (size_t i = 0; i < omegaEvents.getSize(); i++) {
        if (mask & (size_t(1) << i)) {
            event.addElementaryEvent(omegaEvents[i]);
        }
    }

    return event;
}

Event SigmaAlgebra::makeComplement(const Event& event) const {
    Event result;
    const Vector<ElementaryEvent>& omegaEvents = this->omega->getElementaryEvents();
    for (size_t i = 0; i < omegaEvents.getSize(); i++) {
        if (!event.getIdSet().hasNumber(omegaEvents[i].getEventId())) {
            result.addElementaryEvent(omegaEvents[i]);
        }
    }
    return result;
}

Event SigmaAlgebra::makeUnion(const Event& left, const Event& right) const {
    Event result = left;
    result |= right;

    return result;
}

std::ostream& operator << (std::ostream& os, const SigmaAlgebra& sigmaAlgebra) {
    for (size_t i = 0; i < sigmaAlgebra.getContainerOfEvents().getSize(); i++) {
        os << sigmaAlgebra.getContainerOfEvents()[i];
    }
    
    return os;
}
