#include "ElementaryEvent.h"
#include "source/Constants.h"

int32_t ElementaryEvent::idMask = 0;

ElementaryEvent::ElementaryEvent() {
    this->setId();
    this->setEventDescription("Empty elementary event");
}

ElementaryEvent::ElementaryEvent(const char* eventDescription) {
    this->setEventDescription(String(eventDescription));
    this->setId();
}

ElementaryEvent::ElementaryEvent(const String& eventDescription) {
    this->setEventDescription(eventDescription);
    this->setId();
}

void ElementaryEvent::setId() {
    this->realId = this->idMask;
    this->idMask += 1;
}

void ElementaryEvent::setEventDescription(const String& eventDescription) {
    this->eventDescription = eventDescription;
}

int32_t ElementaryEvent::getEventId() const {
    return this->realId;
}

const String &ElementaryEvent::getEventDescription() const {
    return this->eventDescription;
}

ElementaryEvent& ElementaryEvent::operator |= (const ElementaryEvent& other) {
    if (this->getEventId() == other.getEventId()) {
        return *this;
    }

    *this = ElementaryEvent(this->getEventDescription() + String(" ") + RED + "OR" + RESET + " " + other.getEventDescription());
    return *this;
}

ElementaryEvent& ElementaryEvent::operator &= (const ElementaryEvent& other) {
    if (this->getEventId() == other.getEventId()) {
        return *this;
    }

    *this = ElementaryEvent(this->getEventDescription() + String(" ") + RED + "AND" + RESET + " " + other.getEventDescription());
    return *this;
}

ElementaryEvent operator | (const ElementaryEvent& left, const ElementaryEvent& right) {
    ElementaryEvent result = left;
    result |= right;
    return result;
}

ElementaryEvent operator & (const ElementaryEvent& left, const ElementaryEvent& right) {
    ElementaryEvent result = left;
    result &= right;
    return result;
}

std::istream& operator >> (std::istream& is, ElementaryEvent& event) {
    String str;
    is >> str;
    event = ElementaryEvent(str);
    return is;
}

std::ostream& operator << (std::ostream& os, const ElementaryEvent& event) {
    os << "{ [ Event id: " << event.getEventId() << " ], ";
    os << "[ Event description: " << event.getEventDescription() << " ] }";
    return os;
}
