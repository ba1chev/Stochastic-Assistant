#pragma once

#include "source/functions/Function.hpp"
#include "source/sigma_algebra/SigmaAlgebra.h"
#include "source/data_structures/pair/Pair.hpp"
#include "source/data_structures/vector/Vector.hpp"
#include "source/data_structures/interval/Interval.h"
#include "source/events/set_of_elementary_events/event/Event.h"

typedef double (*ProbabilityFunctionFilter)(const Event& event);
Range<double> probabilityFunctionRange();


class ProbabilityFunction: public Function<Event, double> {
private:
    const SigmaAlgebra* sigmaAlgebra = nullptr;
    Vector<Pair<Event, double>> probabilities;

    void fillProbabilities(ProbabilityFunctionFilter filter);
    void setSigmaAlgebraPointer(const SigmaAlgebra* sigmaAlgebra);

public:
    ProbabilityFunction(const SigmaAlgebra* sigmaAlgebra, ProbabilityFunctionFilter filter);
    ProbabilityFunction(const SigmaAlgebra* sigmaAlgebra, const Vector<Pair<Event, double>>& probabilities);

    const SigmaAlgebra* getSigmaAlgebraPointer() const;
    const Vector<Pair<Event, double>>& getProbabilities() const;

    double operator () (const Event& input) const override;
    Function* clone() const override;
};
