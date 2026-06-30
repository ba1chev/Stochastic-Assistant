#pragma once

#include "source/functions/Function.hpp"
#include "source/sigma_algebra/SigmaAlgebra.h"
#include "source/events/set_of_elementary_events/omega/Omega.h"


class ProbabilitySpace {
private:
    const Omega* omega = nullptr;
    const SigmaAlgebra* sigmaAlgebra = nullptr;
    const Function<Event, double>* probabilityFunction = nullptr;

    void setOmega(const Omega* omega);
    void setSigmaAlgebra(const SigmaAlgebra* sigmaAlgebra);
    void setProbabilityFunction(const Function<Event, double>* probaprobabilityFunctionbiltiyFunction);

public:
    ProbabilitySpace(const Omega* omega, const SigmaAlgebra* sigmaAlgebra, const Function<Event, double>* probabilityFunction);

    const Omega* getOmega() const;
    const SigmaAlgebra* getSigmaAlgebra() const;
    const Function<Event, double>* getProbabilityFunction() const;
};
