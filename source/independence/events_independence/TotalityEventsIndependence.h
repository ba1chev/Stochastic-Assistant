#pragma once

#include "source/functions/Function.hpp"
#include "source/data_structures/vector/Vector.hpp"
#include "source/events/set_of_elementary_events/event/Event.h"

Event intersectSubset(const Vector<Event>& events, size_t mask);


class TotalityEventsIndependence {
public:
    bool operator () (const Vector<Event>& containerOfEvents, 
        Function<Event, double>* probabilityFunction) const;
};
