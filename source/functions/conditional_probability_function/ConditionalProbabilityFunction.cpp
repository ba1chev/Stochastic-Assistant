#include <stdexcept>

#include "source/functions/conditional_probability_function/ConditionalProbabilityFunction.h"

ConditionalProbabilityFunction::ConditionalProbabilityFunction(const SigmaAlgebra* sigmaAlgebra, ProbabilityFunctionFilter filter): 
    ProbabilityFunction(sigmaAlgebra, filter) {}

double ConditionalProbabilityFunction::operator () (const Event& left, const Event& right) const {
    const Vector<Event>& containerOfSupportedEvents = this->getSigmaAlgebraPointer()->getContainerOfEvents();
    bool foundLeft = false;
    bool foundRight = false;

    for (size_t i = 0; i < containerOfSupportedEvents.getSize() && !(foundLeft && foundRight); i++) {
        if (!foundLeft && containerOfSupportedEvents[i] == left) foundLeft = true;
        if (!foundRight && containerOfSupportedEvents[i] == right) foundRight = true;
    }

    if (!foundLeft || !foundRight) throw std::logic_error("Input events must be in the same probability space");

    Event intersection;
    const Vector<ElementaryEvent>& leftElementaryEvents = left.getElementaryEvents();
    const BitSet& rightIds = right.getIdSet();

    for (size_t i = 0; i < leftElementaryEvents.getSize(); i++) {
        const ElementaryEvent& current = leftElementaryEvents[i];
        if (rightIds.hasNumber(current.getEventId())) {
            intersection.addElementaryEvent(current);
        }
    }

    double probabilityRight = ProbabilityFunction::operator()(right);
    if (probabilityRight == 0.0) {
        throw std::logic_error("Conditional probability undefined for event with zero probability");
    }

    double probabilityIntersection = ProbabilityFunction::operator()(intersection);
    return probabilityIntersection / probabilityRight;
}
