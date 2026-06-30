#include <stdexcept>
#include <iostream>

#include "Event.h"

Event::Event() {
    this->reset();
}

Event::Event(const Vector<ElementaryEvent>& elementaryEvents) {
    this->reset();
    for (size_t i = 0; i < elementaryEvents.getSize(); i++) {
        this->addElementaryEvent(elementaryEvents[i]);
    }
}

void Event::reset() {
    this->elementaryEvents = Vector<ElementaryEvent>();
    this->idSet = BitSet();
}

size_t Event::findIndexById(int32_t id) const {
    for (size_t i = 0; i < this->elementaryEvents.getSize(); i++) {
        if (this->elementaryEvents[i].getEventId() == id) return i;
    }
    return this->elementaryEvents.getSize();
}

const Vector<ElementaryEvent>& Event::getElementaryEvents() const {
    return this->elementaryEvents;
}

const BitSet& Event::getIdSet() const {
    return this->idSet;
}

void Event::addElementaryEvent(const ElementaryEvent& event) {
    if (this->idSet.hasNumber(event.getEventId())) return;
    this->idSet.addNumber(event.getEventId());
    this->elementaryEvents.push_back(event);
}

void Event::removeEvent(int32_t eventId) {
    size_t index = this->findIndexById(eventId);
    if (index == this->elementaryEvents.getSize()) return;
    this->idSet.removeNumber(eventId);
    this->elementaryEvents.remove_at(index);
}

bool Event::isElementaryEventIn(const ElementaryEvent& event) const {
    return this->idSet.hasNumber(event.getEventId());
}

void Event::clean() {
    this->reset();
}

Event& Event::operator |= (const Event& other) {
    const Vector<ElementaryEvent>& others = other.getElementaryEvents();
    for (size_t i = 0; i < others.getSize(); i++) {
        this->addElementaryEvent(others[i]);
    }
    return *this;
}

Event& Event::operator &= (const Event& other) {
    Vector<ElementaryEvent> filtered;
    const BitSet& otherIds = other.getIdSet();

    for (size_t i = 0; i < this->elementaryEvents.getSize(); i++) {
        const ElementaryEvent& current = this->elementaryEvents[i];
        if (otherIds.hasNumber(current.getEventId())) {
            filtered.push_back(current);
        }
    }

    this->reset();
    for (size_t i = 0; i < filtered.getSize(); i++) {
        this->addElementaryEvent(filtered[i]);
    }

    return *this;
}

Event operator | (const Event& left, const Event& right) {
    Event result = left;
    result |= right;
    return result;
}

Event operator & (const Event& left, const Event& right) {
    Event result = left;
    result &= right;
    return result;
}

bool operator == (const Event& left, const Event& right) {
    const Vector<ElementaryEvent>& leftElems = left.getElementaryEvents();
    const BitSet& rightIds = right.getIdSet();

    if (leftElems.getSize() != right.getElementaryEvents().getSize()) return false;
    for (size_t i = 0; i < leftElems.getSize(); i++) {
        if (!rightIds.hasNumber(leftElems[i].getEventId())) return false;
    }

    return true;
}

bool operator != (const Event& left, const Event& right) {
    return !(left == right);
}

std::ostream& operator << (std::ostream& os, const Event& event) {
    const Vector<ElementaryEvent>& elems = event.getElementaryEvents();

    if (elems.getSize() == 0) {
        os << "{ \n" << "  " << "{ [ Event id: -1 ], [ Event description: Empty event ] }" << "\n" << "}\n";
        return os;
    }

    os << "{ \n";
    for (size_t i = 0; i < elems.getSize(); i++) os << "  " << elems[i] << std::endl;
    os << "}";
    
    return os;
}

std::istream& operator >> (std::istream& is, Event& event) {
    event.clean();
    String currentDescription;
    uint32_t countOfEvents = 0;
    is >> countOfEvents;

    for (size_t i = 0; i < countOfEvents; i++) {
        is >> currentDescription;
        ElementaryEvent currentEvent(currentDescription);
        event.addElementaryEvent(currentEvent);
    }

    return is;
}

const ElementaryEvent& Event::operator [] (size_t index) const {
    return this->elementaryEvents[index];
}

ElementaryEvent& Event::operator [] (size_t index) {
    return this->elementaryEvents[index];
}