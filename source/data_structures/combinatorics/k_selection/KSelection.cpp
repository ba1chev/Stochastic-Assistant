#include <algorithm>

#include "source/data_structures/combinatorics/factoriel/Factoriel.h"
#include "source/data_structures/combinatorics/k_selection/KSelection.h"

KSelection::KSelection(KSelectionPattern buildingPattern, uint32_t n, uint32_t k) {
    this->setN(n);
    this->setK(k);
    this->applyPattern(buildingPattern);
    this->validate();
}

void KSelection::applyPattern(KSelectionPattern buildingPattern) {
    this->setKSelectionPattern(buildingPattern);
    switch (buildingPattern) {
        case KSelectionPattern::Combination: 
            this->orderMatters = false; 
            this->allowRepetition = false; break;
        case KSelectionPattern::CombinationWithRepetition: 
            this->orderMatters = false; 
            this->allowRepetition = true; break;
        case KSelectionPattern::Variation: 
            this->orderMatters = true; 
            this->allowRepetition = false; break;
        case KSelectionPattern::VariationWithRepetition: 
            this->orderMatters = true; 
            this->allowRepetition = true; break;
        case KSelectionPattern::Permutation: 
            this->orderMatters = true; 
            this->allowRepetition = false; 
            this->k = n; break;
        default: throw std::invalid_argument("Unsupported KSelectionPattern");
    }
}

uint64_t KSelection::generateCount() const {
    this->validate();
    switch (this->buildingPattern) {
        case KSelectionPattern::Combination: return HelperFunctions::binomial(this->n, this->k);
        case KSelectionPattern::CombinationWithRepetition: return HelperFunctions::binomial(this->n + this->k - 1, this->k);
        case KSelectionPattern::Variation: return HelperFunctions::permutation(this->n, this->k);
        case KSelectionPattern::VariationWithRepetition: return (uint64_t)std::pow(this->n, this->k);
        case KSelectionPattern::Permutation: return HelperFunctions::permutation(this->n, this->n);
        default: throw std::logic_error("Unsupported pattern");
    }
}

void KSelection::validate() const {
    if (!this->allowRepetition && this->k > this->n) throw std::invalid_argument("k > n is not valid");
}

void KSelection::setN(uint32_t n) {
    this->n = n;
}

void KSelection::setK(uint32_t k) {
    this->k = k;
}

void KSelection::setKSelectionPattern(KSelectionPattern buildingPattern) {
    this->buildingPattern = buildingPattern;
}

uint32_t KSelection::getN() const {
    return this->n;
}

uint32_t KSelection::getK() const {
    return this->k;
}

bool KSelection::getOrderMatters() const {
    return this->orderMatters;
}

bool KSelection::getAllowRepetition() const {
    return this->allowRepetition;
}

KSelectionPattern KSelection::getBuildingPattern() const {
    return this->buildingPattern;
}

uint64_t HelperFunctions::binomial(uint32_t n, uint32_t k) {
    if (k > n) throw std::invalid_argument("k cannot be greater than n");
    uint32_t kSmall = (k > n - k) ? (n - k) : k;
    uint64_t result = 1;

    for (uint32_t i = 1; i <= kSmall; i++) {
        result = result * (n - kSmall + i) / i;
    }

    return result;
}

uint64_t HelperFunctions::permutation(uint32_t n, uint32_t k) {
    if (k > n) throw std::invalid_argument("k cannot be greater than n");
    uint64_t result = 1;

    for (uint32_t i = 0; i < k; i++) {
        result *= (n - i);
    }

    return result;
}