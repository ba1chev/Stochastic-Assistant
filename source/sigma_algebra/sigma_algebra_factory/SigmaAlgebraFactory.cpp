#include <stdexcept>

#include "source/sigma_algebra/SigmaAlgebra.h"
#include "source/sigma_algebra/sigma_algebra_factory/SigmaAlgebraFactory.h"

SigmaAlgebra* SigmaAlgebraFactory::create(SigmaAlgebraPattern pattern, Omega* omega) const {
    switch (pattern) {
        case SigmaAlgebraPattern::Trivial: return new SigmaAlgebra(omega, SigmaAlgebraPattern::Trivial);
        case SigmaAlgebraPattern::PowerSet: return new SigmaAlgebra(omega, SigmaAlgebraPattern::PowerSet);
        default: throw std::logic_error("Not supported sigma-algebra pattern for constructoring");
    }
}
