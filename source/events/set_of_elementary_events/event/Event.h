#pragma once

#include <iosfwd>

#include "source/data_structures/bitset/BitSet.h"
#include "source/data_structures/vector/Vector.hpp"
#include "source/events/elementary_event/ElementaryEvent.h"


class Event {
private:
    Vector<ElementaryEvent> elementaryEvents;
    BitSet idSet;

    void reset();
    size_t findIndexById(int32_t id) const;

public:
    Event();
    explicit Event(const Vector<ElementaryEvent>& elementaryEvents);

    const ElementaryEvent& operator [] (size_t index) const;
    ElementaryEvent& operator [] (size_t index);
    Event& operator &= (const Event& other);
    Event& operator |= (const Event& other);

    const Vector<ElementaryEvent>& getElementaryEvents() const;
    const BitSet& getIdSet() const;

    void addElementaryEvent(const ElementaryEvent& event);
    void removeEvent(int32_t eventId);
    bool isElementaryEventIn(const ElementaryEvent& event) const;
    void clean();

    friend std::ostream& operator << (std::ostream& os, const Event& event);
    friend std::istream& operator >> (std::istream& is, Event& event);
};

Event operator & (const Event& left, const Event& right);
Event operator | (const Event& left, const Event& right);
bool operator == (const Event& left, const Event& right);
bool operator != (const Event& left, const Event& right);
