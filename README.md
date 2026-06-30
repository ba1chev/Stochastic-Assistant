<div align="center">

# Stochastic-Assistant

**A from-scratch C++17 probability library** — probability spaces,
random variables, and joint distributions, with hand-rolled containers
and numerical routines.

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat&logo=c%2B%2B&logoColor=white)

</div>

---

## Build & Run

```sh
cd source
make          # produces ./main
make run      # builds and runs
make clean
```

Requires only `g++` with C++17 support. No external dependencies, no CMake.

`source/main.cpp` is a scratchpad of `test1..test18` demonstrations; `main()`
currently calls `test17` (correlated bivariate Bernoulli) and `test18`
(bivariate Uniform) — edit it to exercise other distributions.

---

## Architecture

The codebase is organised bottom-up in six layers:

```
data_structures   →   events + sigma_algebra   →   functions
                                                       ↓
                                                distributions
                                                       ↓
                                                independence
```

### `source/data_structures/`

Generic primitives written from scratch — no STL containers anywhere:

| Component | Purpose |
| --- | --- |
| `Vector<T>`, `Pair<T,U>`, `String` | Core generic containers |
| `HeterogeneousContainer<T>` | `T**` array preserving dynamic type via `clone()` |
| `BitSet` | Compact set of non-negative integers; `\|=`, `&=`, `%=` are union / intersection / difference |
| `Interval` | Closed $[a, b]$ with union and intersection |
| `Integral` + `TrapezoidalRuleIntegral` | Numerical integration with $n$ sub-intervals (default $10^6$) |
| `special_functions/Gamma` | $\Gamma(x)$ and $\ln\Gamma(x)$ via Lanczos with reflection for $x < 0.5$ |
| `combinatorics/` | `Factoriel` and `KSelection` (combinations, variations, permutations, with/without repetition) |

### `source/events/` and `source/sigma_algebra/`

- **`ElementaryEvent`** — atomic outcome with auto-generated unique id.
- **`Event`** — subset of $\Omega$, backed by both a `Vector<ElementaryEvent>` and
  a `BitSet` for $O(1)$ id-membership tests. `|` and `&` are real set operations.
- **`Omega`** — distinguished event representing the whole sample space.
- **`FullGroupOfEvents`** — a partition of $\Omega$ (hypotheses).
- **`SigmaAlgebra`** + **`SigmaAlgebraFactory`** — generates a $\sigma$-algebra
  from an `Omega` using a `SigmaAlgebraPattern` (`Trivial` $\{\emptyset, \Omega\}$
  or `PowerSet` $2^\Omega$, capped at `POWER_SET_MAX_ELEMENTS = 20`).
- **`ProbabilitySpace`** — the triple $(\Omega, \Sigma, P)$.

### `source/functions/`

A templated callable hierarchy rooted at `Function<T, U>`:

- **`ProbabilityFunction : Function<Event, double>`** — probability measure
  $P: \Sigma \to [0,1]$, stored as a `Vector<Pair<Event, double>>`.
- **`ConditionalProbabilityFunction`** — extends the above with
  $P(A\mid B) = P(A \cap B) / P(B)$.
- **`FullProbabilityFormula`** — composes both into
  $P(A) = \sum_i P(A \mid H_i)\, P(H_i)$.
- **`DensityFunction : Function<double, double>`** — base class for PDFs
  (`UniformDensityFunction`, `NormalDensityFunction`, `ExponentialDensityFunction`,
  `GammaDensityFunction`, `ChiSquaredDensityFunction`, `StudentTDensityFunction`).
  Heavy PDFs use a log-trick through `logGamma` for numerical stability.

### `source/distributions/`

Random variables built on top of the function layer. `RandomVariable<T>` exposes
`calculateProbability`, `getExpectation`, `getVariance`, `getOverallType`.

<table>
<tr><th align="left">Discrete</th><th align="left">Continuous</th></tr>
<tr><td>

`Bernoulli(p)` &nbsp; $P(X=1)=p$

`Binomial(n,p)` &nbsp; $P(X=k)=\binom{n}{k}p^k(1-p)^{n-k}$

`Geometric(p)` &nbsp; $P(X=k)=p(1-p)^k$

`NegativeBinomial(r,p)` &nbsp; $P(X=k)=\binom{k+r-1}{r-1}p^r(1-p)^k$

`Poisson(λ)` &nbsp; $P(X=k)=\frac{\lambda^k e^{-\lambda}}{k!}$

`HyperGeometric(N,K,n)` &nbsp; $P(X=k)=\frac{\binom{K}{k}\binom{N-K}{n-k}}{\binom{N}{n}}$

</td><td>

`Uniform(a,b)` &nbsp; $f(x) = \frac{1}{b-a}$

`Normal(μ,σ²)` &nbsp; $f(x)=\frac{1}{\sigma\sqrt{2\pi}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$

`Exponential(λ)` &nbsp; $f(x)=\lambda e^{-\lambda x}$

`Gamma(α,β)` &nbsp; $f(x)=\frac{\beta^\alpha}{\Gamma(\alpha)}x^{\alpha-1}e^{-\beta x}$

`ChiSquared(k)` &nbsp; $f(x)=\frac{1}{2^{k/2}\Gamma(k/2)}x^{k/2-1}e^{-x/2}$

`StudentT(ν)` &nbsp; $f(t)\propto\left(1+\frac{t^2}{\nu}\right)^{-(\nu+1)/2}$

</td></tr>
</table>

`Bernoulli`, `Binomial`, `Geometric`, and `NegativeBinomial` share the
`BernoulliSchemeRandomVariable<T>` base. Continuous distributions evaluate
$P(X \in [a,b])$ by trapezoidal integration of their density; unbounded supports
are truncated using the cutoff multipliers in `Constants.h`.

**Joint distributions** live in `distributions/joint_distributions/`:

- **`JointDiscreteDistribution<T>`** — marginals + per-variable supports +
  either an explicit PMF table or a functor. Exposes `jointProbability`,
  `marginal`, `conditional`, `covariance`, `correlation`, `areIndependent`.
- **`JointContinuousDistribution`** — marginals + supports + PDF functor. Uses
  tensor trapezoidal integration over each dimension.

The mode of construction is captured by `JointDistributionMode`
(`Independent` / `TableGeneral` / `FunctorGeneral`).

### `source/independence/`

- **`EventsIndependence`** — pairwise: $|P(A \cap B) - P(A)P(B)| \le \varepsilon$.
- **`TotalityEventsIndependence`** — mutual independence over $k$ events;
  enumerates all non-trivial subsets via bitmasks and verifies
  $P\bigl(\bigcap_{i \in S} A_i\bigr) = \prod_{i \in S} P(A_i)$.

---

## Tuning

All numerical knobs live in `source/Constants.h`:

| Constant | Purpose |
| --- | --- |
| `EPS`, `INDEPENDENCE_CHECK_TOLERANCE` | Floating-point comparison tolerances |
| `COUNT_OF_SUB_INTERVALS` | Trapezoidal rule resolution ($10^6$) |
| `POWER_SET_MAX_ELEMENTS`, `SIGMA_ALGEBRA_SIZE_LIMIT` | Combinatorial guards |
| `*_CUTOFF_MULTIPLIER` | How far past the mean to truncate unbounded supports |
| `JOINT_CONTINUOUS_STEPS_PER_DIMENSION` | Resolution of tensor integration |

---

## Example

```cpp
ContinuousRandomVariable<double>* normal = new Normal(0, 1);

std::cout << normal->getExpectation() << "\n";              // 0
std::cout << normal->getVariance() << "\n";                 // 1
std::cout << normal->calculateProbability(Interval(-1, 1)); // ≈ 0.6827

delete normal;
```

See `source/main.cpp` for further worked examples covering every distribution
and both joint distribution flavours.
