#pragma once

#include "source/data_structures/vector/Vector.hpp"
#include "source/events/set_of_elementary_events/event/Event.h"
#include "source/functions/probability_function/ProbabilityFunction.h"
#include "source/functions/conditional_probability_function/ConditionalProbabilityFunction.h"


class FullProbabilityFormula {
private:
    Event event;

public:
    FullProbabilityFormula(const Event& event);
    double operator()(const Vector<Event>& hypothesis, 
        const ProbabilityFunction& probabilityFunction, 
        const ConditionalProbabilityFunction& conditionalProbabilityFunction) const;
};
