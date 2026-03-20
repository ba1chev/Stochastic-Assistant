#include "source/sigma_algebra/SigmaAlgebra.h"
#include "source/sigma_algebra/sigma_algebra_factory/SigmaAlgebraFactory.h"
#include "source/functions/probability_function/ProbabilityFunction.h"
#include "source/functions/conditional_probability_function/ConditionalProbabilityFunction.h"
#include "source/distributions/discrete_distributions/binomial/Binomial.h"
#include "source/distributions/discrete_distributions/BernoulliSchemeRandomVariable.hpp"
#include "source/Constants.h"
#include "source/functions/Function.hpp"
#include "source/distributions/discrete_distributions/geometric/Geometric.h"
#include "source/distributions/discrete_distributions/negative_binomial/NegativeBinomial.h"
#include "source/functions/density_function/uniform_density_function/UniformDensityFunction.h"
#include "source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.h"
#include "source/data_structures/interval/Interval.h"
#include "source/distributions/continuous_distributions/uniform/Uniform.h"
#include "source/distributions/continuous_distributions/normal/Normal.h"
#include "source/distributions/continuous_distributions/exponential/Exponential.h"
#include "source/distributions/discrete_distributions/poisson/Poisson.h"
#include "source/distributions/discrete_distributions/hyper_geometric/HyperGeometric.h"
#include <iostream>


void test1() {
    size_t countOfExperimenents = 10;
    BernoulliSchemeRandomVariable<bool>* ber1 = new Bernoulli(0.9);
    BernoulliSchemeRandomVariable<bool>* ber2 = new Bernoulli(0.5);

    HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>> bersFair;
    HeterogeneousContainer<BernoulliSchemeRandomVariable<bool>> bersUnFair;
    for (size_t i = 0; i < countOfExperimenents; i++) bersFair.addElement(ber2);
    for (size_t i = 0; i < countOfExperimenents; i++) bersUnFair.addElement(ber1);
    
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
    ElementaryEvent elementartyEvent1("I studided hard for statistics exam");
    ElementaryEvent elementartyEvent2("I passed statistics exam");
    ElementaryEvent elementartyEvent3("I did not passed statistics exam");

    ElementaryEvent result1 = elementartyEvent1 | elementartyEvent2;
    ElementaryEvent result2 = elementartyEvent1 & elementartyEvent2;
    std::cout << result1 << std::endl;
    std::cout << result2 << std::endl;

    Vector<ElementaryEvent> container1;
    Vector<ElementaryEvent> container2;

    container1.push_back(elementartyEvent1);
    container1.push_back(elementartyEvent2);
    container2.push_back(elementartyEvent1);
    container2.push_back(elementartyEvent3);

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
    Integral* integral = new TrapezoidalRuleIntergral(function, 10000);
    std::cout << integral->intergrate(1.5, 3) << std::endl;

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

int main() {
    test13();
    return 0;
}
