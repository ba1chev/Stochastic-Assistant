#include "source/data_structures/combinatorics/factoriel/Factoriel.h"

size_t Factorial::operator () (size_t number) const {
    if (number == 1 || number == 0) return 1;
    return number * this->operator()(number - 1);
}