#pragma once

#include "source/functions/Function.hpp"
#include "source/events/set_of_elementary_events/event/Event.h"


class EventsIndependence {
public:
    bool operator () (const Event& left, const Event& right, 
        Function<Event, double>* probabilityFunction) const;
};