#include <algorithm>

#include "Interval.h"

Interval::Interval(double left, double right) {
    if (left > right) throw std::runtime_error("Right component must be greater than left one");
    this->setLeftComponent(left);
    this->setRightComponent(right);
}

Interval& Interval::operator |= (const Interval& other) {
    this->setLeftComponent(std::min(this->left, other.getLeftComponent()));
    this->setRightComponent(std::max(this->right, other.getRightComponent()));
    return *this;
}

Interval& Interval::operator &= (const Interval& other) {
    this->setLeftComponent(std::max(this->left, other.getLeftComponent()));
    this->setRightComponent(std::min(this->right, other.getRightComponent()));
    return *this;
}

bool Interval::isInInterval(double number) const {
    return number >= this->left && number <= this->right;
}

double Interval::getLeftComponent() const {
    return this->left;
}

double Interval::getRightComponent() const {
    return this->right;
}

Interval operator & (const Interval& left, const Interval& right) {
    Interval result = left;
    result &= right;
    return result;
}

Interval operator | (const Interval& left, const Interval& right) {
    Interval result = left;
    result |= right;
    return result;
}

void Interval::setLeftComponent(double left) {
    this->left = left;
}

void Interval::setRightComponent(double right) {
    this->right = right;
}