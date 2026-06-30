#include <stdexcept>

#include "source/functions/conditional_probability_function/full_probability_formula/FullProbabilityFormula.h"

FullProbabilityFormula::FullProbabilityFormula(const Event& event): event(event) {}

double FullProbabilityFormula::operator()(const Vector<Event>& hypothesis, 
    const ProbabilityFunction& probabilityFunction, const ConditionalProbabilityFunction& conditionalProbabilityFunction) const {
    if (hypothesis.getSize() == 0) throw std::logic_error("Full probability formula requires a non-empty hypothesis set");

    double result = 0.0;
    for (size_t i = 0; i < hypothesis.getSize(); i++) {
        const Event& h = hypothesis[i];
        double pH = probabilityFunction(h);
        double pAgivenH = conditionalProbabilityFunction(this->event, h);
        result += pAgivenH * pH;
    }
    return result;
}
