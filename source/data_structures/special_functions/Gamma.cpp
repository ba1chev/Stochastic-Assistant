#include "source/data_structures/special_functions/Gamma.h"
#include "source/Constants.h"
#include <cmath>
#include <stdexcept>

namespace {
    constexpr double LANCZOS_G = 7.0;
    constexpr double LANCZOS_COEFFICIENTS[9] = {
        0.99999999999980993,
        676.5203681218851,
        -1259.1392167224028,
        771.32342877765313,
        -176.61502916214059,
        12.507343278686905,
        -0.13857109526572012,
        9.9843695780195716e-6,
        1.5056327351493116e-7
    };
}

double SpecialFunctions::logGamma(double x) {
    if (x <= 0.0) throw std::logic_error("logGamma requires positive argument");

    if (x < 0.5) {
        return std::log(PI / std::sin(PI * x)) - SpecialFunctions::logGamma(1.0 - x);
    }

    x -= 1.0;
    double a = LANCZOS_COEFFICIENTS[0];
    for (int i = 1; i < 9; i++) {
        a += LANCZOS_COEFFICIENTS[i] / (x + i);
    }

    const double t = x + LANCZOS_G + 0.5;
    return 0.5 * std::log(2.0 * PI) + (x + 0.5) * std::log(t) - t + std::log(a);
}

double SpecialFunctions::gamma(double x) {
    return std::exp(SpecialFunctions::logGamma(x));
}
