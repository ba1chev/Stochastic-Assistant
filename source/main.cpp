#include <limits>
#include <iostream>

#include "source/Constants.h"
#include "source/functions/Function.hpp"
#include "source/sigma_algebra/SigmaAlgebra.h"
#include "source/data_structures/interval/Interval.h"
#include "source/functions/probability_function/ProbabilityFunction.h"
#include "source/distributions/continuous_distributions/gamma/Gamma.h"
#include "source/distributions/discrete_distributions/poisson/Poisson.h"
#include "source/distributions/continuous_distributions/normal/Normal.h"
#include "source/distributions/continuous_distributions/uniform/Uniform.h"
#include "source/distributions/discrete_distributions/binomial/Binomial.h"
#include "source/sigma_algebra/sigma_algebra_factory/SigmaAlgebraFactory.h"
#include "source/distributions/discrete_distributions/bernoulii/Bernoulli.h"
#include "source/distributions/discrete_distributions/geometric/Geometric.h"
#include "source/distributions/continuous_distributions/student_t/StudentT.h"
#include "source/distributions/continuous_distributions/chi_squared/ChiSquared.h"
#include "source/distributions/continuous_distributions/exponential/Exponential.h"
#include "source/distributions/discrete_distributions/hyper_geometric/HyperGeometric.h"
#include "source/distributions/discrete_distributions/BernoulliSchemeRandomVariable.hpp"
#include "source/distributions/discrete_distributions/negative_binomial/NegativeBinomial.h"
#include "source/functions/conditional_probability_function/ConditionalProbabilityFunction.h"
#include "source/functions/density_function/uniform_density_function/UniformDensityFunction.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/distributions/joint_distributions/discrete_distribution/JointDiscreteDistribution.hpp"
#include "source/distributions/joint_distributions/continuous_distribution/JointContinuousDistribution.hpp"

void test1() {
    size_t countOfExperiments = 10;
    BernoulliSchemeRandomVariable<bool>* ber1 = new Bernoulli(0.9);
    BernoulliSchemeRandomVariable<bool>* ber2 = new Bernoulli(0.5);

    HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>> bersFair;
    HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>> bersUnFair;
    for (size_t i = 0; i < countOfExperiments; i++) bersFair.addElement(ber2);
    for (size_t i = 0; i < countOfExperiments; i++) bersUnFair.addElement(ber1);
    
    BernoulliSchemeRandomVariable<uint32_t>* binFair = new Binomial(bersFair);
    BernoulliSchemeRandomVariable<uint32_t>* binUnFair = new Binomial(bersUnFair);
    std::cout << binFair->calculateProbability(5) << std::endl;
    std::cout << binUnFair->calculateProbability(5) << std::endl;

    delete ber1;
    delete ber2;
    delete binFair;
    delete binUnFair;
    ber1 = nullptr;
    ber2 = nullptr;
    binFair = nullptr;
    binUnFair = nullptr;
}

void test2() {
    ElementaryEvent elementaryEvent1("I studied hard for statistics exam");
    ElementaryEvent elementaryEvent2("I passed statistics exam");
    ElementaryEvent elementaryEvent3("I did not pass statistics exam");

    ElementaryEvent result1 = elementaryEvent1 | elementaryEvent2;
    ElementaryEvent result2 = elementaryEvent1 & elementaryEvent2;
    std::cout << result1 << std::endl;
    std::cout << result2 << std::endl;

    Vector<ElementaryEvent> container1;
    Vector<ElementaryEvent> container2;

    container1.push_back(elementaryEvent1);
    container1.push_back(elementaryEvent2);
    container2.push_back(elementaryEvent1);
    container2.push_back(elementaryEvent3);

    Event event1(container1);
    Event event2(container2);
    std::cout << (event1 & event2) << std::endl;
    std::cout << (event1 | event2) << std::endl;
}

void test3() {
    ElementaryEvent e1("It is 1");
    ElementaryEvent e2("It is 2");
    ElementaryEvent e3("It is 3");
    ElementaryEvent e4("It is 4");
    ElementaryEvent e5("It is 5");
    ElementaryEvent e6("It is 6");

    Event eventFour;
    eventFour.addElementaryEvent(e4);

    Event eventEven;
    eventEven.addElementaryEvent(e2);
    eventEven.addElementaryEvent(e4);
    eventEven.addElementaryEvent(e6);

    Event eventOdd;
    eventOdd.addElementaryEvent(e1);
    eventOdd.addElementaryEvent(e3);
    eventOdd.addElementaryEvent(e5);

    Omega* omega = new Omega();
    omega->addElementaryEvent(e1);
    omega->addElementaryEvent(e2);
    omega->addElementaryEvent(e3);
    omega->addElementaryEvent(e4);
    omega->addElementaryEvent(e5);
    omega->addElementaryEvent(e6);

    SigmaAlgebraAbstractFactory* factory = new SigmaAlgebraFactory();
    SigmaAlgebra* sigmaAlgebra = factory->create(SigmaAlgebraPattern::PowerSet, omega);

    auto diceProbability = [](const Event& event) -> double {
        return (double)(event.getElementaryEvents().getSize()) / 6.0;
    };

    ProbabilityFunction probabilityFunction(sigmaAlgebra, diceProbability);
    ConditionalProbabilityFunction conditionalProbabilityFunction(sigmaAlgebra, diceProbability);

    double pEven = probabilityFunction(eventEven);
    double pOdd = probabilityFunction(eventOdd);
    double p4GivenEven = conditionalProbabilityFunction(eventFour, eventEven);
    double p4GivenOdd = conditionalProbabilityFunction(eventFour, eventOdd);

    double result = p4GivenEven * pEven + p4GivenOdd * pOdd;
    std::cout << "P({It is 4}) = " << result << std::endl;

    delete omega;
    delete factory;
    delete sigmaAlgebra;
    omega = nullptr;
    factory = nullptr;
    sigmaAlgebra = nullptr;
}

void test4() {
    Function<double, double>* function = new UniformDensityFunction(Interval(0, 3));
    Integral* integral = new TrapezoidalRuleIntegral(function, 10000);
    std::cout << integral->integrate(1.5, 3) << std::endl;

    delete function;
    delete integral;
    function = nullptr;
    integral = nullptr;
}

void test5() {
    ContinuousRandomVariable<double>* uniform = new Uniform(0 ,1);
    std::cout << "The expectation is: " << uniform->getExpectation() << std::endl;
    std::cout << "The variance is: " << uniform->getVariance() << std::endl;
    std::cout << uniform->calculateProbability(Interval(0, 0.5)) << std::endl;

    delete uniform;
    uniform = nullptr;
}

void test6() {
    ContinuousRandomVariable<double>* normal = new Normal(0 ,1);
    std::cout << "The expectation is: " << normal->getExpectation() << std::endl;
    std::cout << "The variance is: " << normal->getVariance() << std::endl;
    std::cout << normal->calculateProbability(Interval(-0.5, 1)) << std::endl;

    delete normal;
    normal = nullptr;
}

void test7() {
    ContinuousRandomVariable<double>* exponential = new Exponential(3);
    std::cout << "The expectation is: " << exponential->getExpectation() << std::endl;
    std::cout << "The variance is: " << exponential->getVariance() << std::endl;
    std::cout << exponential->calculateProbability(Interval(0, 1)) << std::endl;

    delete exponential;
    exponential = nullptr;
}

void test8() {
    BernoulliSchemeRandomVariable<bool>* bernoulli = new Bernoulli(0.5);
    std::cout << "The expectation is: " << bernoulli->getExpectation() << std::endl;
    std::cout << "The variance is: " << bernoulli->getVariance() << std::endl;
    std::cout << bernoulli->calculateProbability(0) << std::endl;
    std::cout << bernoulli->calculateProbability(1) << std::endl;

    delete bernoulli;
    bernoulli = nullptr;
}

void test9() {
    BernoulliSchemeRandomVariable<uint32_t>* binomial = new Binomial(10, 0.5);
    std::cout << "The expectation is: " << binomial->getExpectation() << std::endl;
    std::cout << "The variance is: " << binomial->getVariance() << std::endl;
    for (size_t i = 0; i <= 10; i++) std::cout << binomial->calculateProbability(i) << std::endl;

    delete binomial;
    binomial = nullptr;
}

void test10() {
    BernoulliSchemeRandomVariable<uint32_t>* geometric = new Geometric(0.5);
    std::cout << "The expectation is: " << geometric->getExpectation() << std::endl;
    std::cout << "The variance is: " << geometric->getVariance() << std::endl;
    for (size_t i = 0; i <= 10; i++) std::cout << geometric->calculateProbability(i) << std::endl;

    delete geometric;
    geometric = nullptr;
}

void test11() {
    BernoulliSchemeRandomVariable<uint32_t>* negativeBinomial = new NegativeBinomial(10, 0.5);
    std::cout << "The expectation is: " << negativeBinomial->getExpectation() << std::endl;
    std::cout << "The variance is: " << negativeBinomial->getVariance() << std::endl;
    for (size_t i = 0; i <= 10; i++) std::cout << negativeBinomial->calculateProbability(i) << std::endl;

    delete negativeBinomial;
    negativeBinomial = nullptr;
}

void test12() {
    DiscreteRandomVariable<uint32_t>* poisson = new Poisson(5);
    std::cout << "The expectation is: " << poisson->getExpectation() << std::endl;
    std::cout << "The variance is: " << poisson->getVariance() << std::endl;
    for (size_t i = 0; i <= 10; i++) std::cout << poisson->calculateProbability(i) << std::endl;

    delete poisson;
    poisson = nullptr;
}

void test13() {
    DiscreteRandomVariable<uint32_t>* hyperGeometric = new HyperGeometric(100, 50, 10);
    std::cout << "The expectation is: " << hyperGeometric->getExpectation() << std::endl;
    std::cout << "The variance is: " << hyperGeometric->getVariance() << std::endl;
    for (size_t i = 0; i <= 10; i++) std::cout << hyperGeometric->calculateProbability(i) << std::endl;

    delete hyperGeometric;
    hyperGeometric = nullptr;
}

void test14() {
    ContinuousRandomVariable<double>* gamma = new Gamma(2.0, 0.5);
    std::cout << "The expectation is: " << gamma->getExpectation() << std::endl;
    std::cout << "The variance is: " << gamma->getVariance() << std::endl;
    std::cout << gamma->calculateProbability(Interval(0, 4)) << std::endl;
    std::cout << gamma->calculateProbability(Interval(0, std::numeric_limits<double>::infinity())) << std::endl;

    delete gamma;
    gamma = nullptr;
}

void test15() {
    ContinuousRandomVariable<double>* chiSquared = new ChiSquared(5);
    std::cout << "The expectation is: " << chiSquared->getExpectation() << std::endl;
    std::cout << "The variance is: " << chiSquared->getVariance() << std::endl;
    std::cout << chiSquared->calculateProbability(Interval(0, 5)) << std::endl;
    std::cout << chiSquared->calculateProbability(Interval(0, std::numeric_limits<double>::infinity())) << std::endl;

    delete chiSquared;
    chiSquared = nullptr;
}

void test16() {
    ContinuousRandomVariable<double>* studentT = new StudentT(10);
    std::cout << "The expectation is: " << studentT->getExpectation() << std::endl;
    std::cout << "The variance is: " << studentT->getVariance() << std::endl;
    std::cout << studentT->calculateProbability(Interval(-1, 1)) << std::endl;
    std::cout << studentT->calculateProbability(Interval(-std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity())) << std::endl;

    delete studentT;
    studentT = nullptr;
}

double bivariateUniformPDF(const Vector<double>& point) {
    if (point[0] < 0.0 || point[0] > 1.0) return 0.0;
    if (point[1] < 0.0 || point[1] > 1.0) return 0.0;
    return 1.0;
}

double bivariateCorrelatedPMF(const Vector<uint32_t>& point) {
    if (point[0] == 0 && point[1] == 0) return 0.4;
    if (point[0] == 0 && point[1] == 1) return 0.1;
    if (point[0] == 1 && point[1] == 0) return 0.1;
    if (point[0] == 1 && point[1] == 1) return 0.4;
    return 0.0;
}

void test17() {
    Vector<Pair<Vector<uint32_t>, double>> table;
    Vector<uint32_t> p00; p00.push_back(0); p00.push_back(0);
    Vector<uint32_t> p01; p01.push_back(0); p01.push_back(1);
    Vector<uint32_t> p10; p10.push_back(1); p10.push_back(0);
    Vector<uint32_t> p11; p11.push_back(1); p11.push_back(1);
    table.push_back(Pair<Vector<uint32_t>, double>(p00, 0.4));
    table.push_back(Pair<Vector<uint32_t>, double>(p01, 0.1));
    table.push_back(Pair<Vector<uint32_t>, double>(p10, 0.1));
    table.push_back(Pair<Vector<uint32_t>, double>(p11, 0.4));

    Vector<uint32_t> supp; supp.push_back(0); supp.push_back(1);
    Vector<Vector<uint32_t>> supports; supports.push_back(supp); supports.push_back(supp);

    BernoulliSchemeRandomVariable<uint32_t>* b1 = new Binomial(1, 0.5);
    BernoulliSchemeRandomVariable<uint32_t>* b2 = new Binomial(1, 0.5);
    HeterogeneousContainer<RandomVariable<uint32_t>> marginals;
    marginals.addElement(b1);
    marginals.addElement(b2);

    JointDiscreteDistribution<uint32_t> joint(marginals, supports, table);
    std::cout << "P(X=0, Y=0) = " << joint.jointProbability(p00) << std::endl;
    std::cout << "P(X=1, Y=1) = " << joint.jointProbability(p11) << std::endl;
    Vector<Pair<uint32_t, double>> margX = joint.marginal(0);
    for (size_t i = 0; i < margX.getSize(); i++) {
        std::cout << "P(X=" << margX[i].getFirstComponent() << ") = "
            << margX[i].getSecondComponent() << std::endl;
    }
    std::cout << "Cov(X, Y) = " << joint.covariance(0, 1) << std::endl;
    std::cout << "Corr(X, Y) = " << joint.correlation(0, 1) << std::endl;
    std::cout << "Independent: " << (joint.areIndependent() ? "yes" : "no") << std::endl;

    delete b1; delete b2;
}

void test18() {
    ContinuousRandomVariable<double>* u1 = new Uniform(0, 1);
    ContinuousRandomVariable<double>* u2 = new Uniform(0, 1);
    HeterogeneousContainer<RandomVariable<Interval>> marginals;
    marginals.addElement(u1);
    marginals.addElement(u2);

    Vector<Interval> supports;
    supports.push_back(Interval(0, 1));
    supports.push_back(Interval(0, 1));

    JointContinuousDistribution joint(marginals, supports, bivariateUniformPDF);
    Vector<Interval> region;
    region.push_back(Interval(0, 0.5));
    region.push_back(Interval(0, 0.5));
    std::cout << "P([0,0.5] x [0,0.5]) = " << joint.jointProbability(region) << std::endl;
    std::cout << "Marginal P(X in [0, 0.5]) = " << joint.marginalProbability(0, Interval(0, 0.5)) << std::endl;
    std::cout << "Cov(X, Y) = " << joint.covariance(0, 1) << std::endl;
    std::cout << "Independent: " << (joint.areIndependent() ? "yes" : "no") << std::endl;

    delete u1; delete u2;
}

int main() {
    std::cout << "=== JointDiscrete (correlated Bernoullis) ===" << std::endl;
    test17();
    std::cout << "=== JointContinuous (bivariate Uniform) ===" << std::endl;
    test18();
    return 0;
}
