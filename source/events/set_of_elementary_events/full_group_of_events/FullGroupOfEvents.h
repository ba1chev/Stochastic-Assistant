#pragma once

#include "source/events/set_of_elementary_events/event/Event.h"
#include "source/events/set_of_elementary_events/omega/Omega.h"
#include "source/independence/events_independence/TotalityEventsIndependence.h"


class FullGroupOfEvents {
private:
    const Omega* omega = nullptr;
    Vector<Event> hypothesis;

    bool areIndependent() const;
    bool areTheyFillOmega() const;
    void setOmegaRef(const Omega* omega);
    void setHypothesis(const Vector<Event>& hypothesis);
  
public:
    explicit FullGroupOfEvents(const Vector<Event>& hypothesis, const Omega* omega);

    const Event& operator [] (size_t index) const;
    Event& operator [] (size_t index);
};
