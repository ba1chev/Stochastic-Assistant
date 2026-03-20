CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I..

TARGET := main

SRC := \
	main.cpp \
	data_structures/string/String.cpp \
	data_structures/bitset/BitSet.cpp \
	data_structures/combinatorics/factoriel/Factoriel.cpp \
	data_structures/combinatorics/k_selection/KSelection.cpp \
	data_structures/interval/Interval.cpp \
	events/elementary_event/ElementaryEvent.cpp \
	events/set_of_elementary_events/event/Event.cpp \
	sigma_algebra/SigmaAlgebra.cpp \
	sigma_algebra/sigma_algebra_factory/SigmaAlgebraFactory.cpp \
	functions/probability_function/ProbabilityFunction.cpp \
	functions/density_function/DensityFunction.cpp \
	distributions/discrete_distributions/bernoulii/Bernoulli.cpp \
	distributions/discrete_distributions/binomial/Binomial.cpp \
	distributions/discrete_distributions/geometric/Geometric.cpp \
	distributions/discrete_distributions/negative_binomial/NegativeBinomial.cpp \
	distributions/discrete_distributions/poisson/Poisson.cpp \
	data_structures/integration/Integral.cpp \
	distributions/discrete_distributions/hyper_geometric/HyperGeometric.cpp \
	events/probability_space/ProbabilitySpace.cpp \
	functions/conditional_probability_function/ConditionalProbabilityFunction.cpp \
	functions/conditional_probability_function/full_probability_formula/FullProbabilityFormula.cpp \
	events/set_of_elementary_events/full_group_of_events/FullGroupOfEvents.cpp \
	data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.cpp \
	functions/density_function/uniform_density_function/UniformDensityFunction.cpp \
	functions/density_function/normal_density_function/NormalDensityFunction.cpp \
	distributions/continuous_distributions/uniform/Uniform.cpp \
	distributions/continuous_distributions/normal/Normal.cpp \
	functions/density_function/exponential_density_function/ExponentialDensityFunction.cpp \
	distributions/continuous_distributions/exponential/Exponential.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf $(TARGET).dSYM
