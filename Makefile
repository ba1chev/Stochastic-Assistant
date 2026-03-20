CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -I.

TARGET := main

SRC := \
	main.cpp \
	source/data_structures/string/String.cpp \
	source/data_structures/bitset/BitSet.cpp \
	source/data_structures/combinatorics/factoriel/Factoriel.cpp \
	source/data_structures/combinatorics/k_selection/KSelection.cpp \
	source/data_structures/interval/Interval.cpp \
	source/events/elementary_event/ElementaryEvent.cpp \
	source/events/set_of_elementary_events/event/Event.cpp \
	source/sigma_algebra/SigmaAlgebra.cpp \
	source/sigma_algebra/sigma_algebra_factory/SigmaAlgebraFactory.cpp \
	source/functions/probability_function/ProbabilityFunction.cpp \
	source/functions/density_function/DensityFunction.cpp \
	source/distributions/discrete_distributions/bernoulii/Bernoulli.cpp \
	source/distributions/discrete_distributions/binomial/Binomial.cpp \
	source/distributions/discrete_distributions/geometric/Geometric.cpp \
	source/distributions/discrete_distributions/negative_binomial/NegativeBinomial.cpp \
	source/distributions/discrete_distributions/poisson/Poisson.cpp \
	source/data_structures/integration/Integral.cpp \
	source/distributions/discrete_distributions/hyper_geometric/HyperGeometric.cpp \
	source/events/probability_space/ProbabilitySpace.cpp \
	source/functions/conditional_probability_function/ConditionalProbabilityFunction.cpp \
	source/functions/conditional_probability_function/full_probability_formula/FullProbabilityFormula.cpp \
	source/events/set_of_elementary_events/full_group_of_events/FullGroupOfEvents.cpp \
	source/data_structures/integration/trapezoidal_rule_intergral/TrapezoidalRuleIntergral.cpp \
	source/functions/density_function/uniform_density_function/UniformDensityFunction.cpp \
	source/functions/density_function/normal_density_function/NormalDensityFunction.cpp \
	source/distributions/continuous_distributions/uniform/Uniform.cpp \
	source/distributions/continuous_distributions/normal/Normal.cpp \
	source/functions/density_function/exponential_density_function/ExponentialDensityFunction.cpp \
	source/distributions/continuous_distributions/exponential/Exponential.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
