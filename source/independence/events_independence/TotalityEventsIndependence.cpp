#include <cmath>
#include <stdexcept>

#include "source/Constants.h"
#include "source/functions/probability_function/ProbabilityFunction.h"
#include "source/independence/events_independence/TotalityEventsIndependence.h"

Event intersectSubset(const Vector<Event>& events, size_t mask) {
    Event result = Event();
    bool first = true;

    for (size_t i = 0; mask; i++, mask >>= 1) {
        if (mask & 1) {
            if (first) { result = events[i]; first = false; }
            else result &= events[i];
        }
    }
    
    return result;
}

bool TotalityEventsIndependence::operator () (const Vector<Event>& containerOfEvents, 
    Function<Event, double>* probabilityFunction) const {
    if (!probabilityFunction) throw std::logic_error("Probability function pointer is null");
    if (containerOfEvents.getSize() == 0) throw std::logic_error("Independence check requires at least one event");

    size_t countOfEvents = containerOfEvents.getSize();
    size_t subsetCount = 1u << countOfEvents;
    for (size_t mask = 1; mask < subsetCount; mask++) {
        if ((mask & (mask - 1)) == 0) continue;

        double product = 1.0;
        for (size_t i = 0; i < countOfEvents; i++) {
            if (mask & (1u << i)) {
                product *= probabilityFunction->operator()(containerOfEvents[i]);
            }
        }

        Event intersection = intersectSubset(containerOfEvents, mask);
        double pIntersection = probabilityFunction->operator()(intersection);
        if (std::fabs(product - pIntersection) > EPS) return false;
    }

    return true;
}