#include "source/events/set_of_elementary_events/full_group_of_events/FullGroupOfEvents.h"
#include <stdexcept>

FullGroupOfEvents::FullGroupOfEvents(const Vector<Event>& hypothesis, const Omega* omega) {
    this->setOmegaRef(omega);
    this->setHypothesis(hypothesis);
}

void FullGroupOfEvents::setOmegaRef(const Omega* omega) {
    if (!omega) throw std::runtime_error("Nullptr detected");
    this->omega = omega;
}

void FullGroupOfEvents::setHypothesis(const Vector<Event>& hypothesis) {
    this->hypothesis = hypothesis;
}

bool FullGroupOfEvents::areIndependent() const {
    for (size_t i = 0; i < hypothesis.getSize(); i++) {
        for (size_t j = i + 1; j < hypothesis.getSize(); j++) {
            Event intersection = hypothesis[i] & hypothesis[j];
            if (intersection.getElementaryEvents().getSize() != 0) {
                return false;
            }
        }
    }
    return true;
}

bool FullGroupOfEvents::areTheyFillOmega() const {
    if (!omega) {
        throw std::logic_error("Omega pointer is null");
    }

    BitSet coverage;
    const Vector<ElementaryEvent>& omegaEvents = omega->getElementaryEvents();
    for (size_t i = 0; i < hypothesis.getSize(); i++) {
        const Vector<ElementaryEvent>& elems = hypothesis[i].getElementaryEvents();
        for (size_t j = 0; j < elems.getSize(); j++) {
            coverage.addNumber(elems[j].getEventId());
        }
    }

    for (size_t i = 0; i < omegaEvents.getSize(); i++) {
        if (!coverage.hasNumber(omegaEvents[i].getEventId())) {
            return false;
        }
    }

    return true;
}
