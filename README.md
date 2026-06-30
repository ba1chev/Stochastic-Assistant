# Stochastic-Assistant

A C++17 library for modelling probability spaces, random variables, and joint
distributions from first principles. Built without STL containers — every data
structure (`Vector`, `String`, `BitSet`, `HeterogeneousContainer`, …) and every
numerical routine (trapezoidal integration, Lanczos approximation of $\Gamma$,
combinatorics) is implemented in-house.

## Build & run

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

## Architecture

The codebase is organised bottom-up in six layers:

```
data_structures   →   events + sigma_algebra   →   functions
                                                       ↓
                                                distributions
                                                       ↓
                                                independence
```

### 1. `source/data_structures/`
Generic primitives written from scratch:
- `Vector<T>`, `Pair<T,U>`, `String`, `HeterogeneousContainer<T>` (a `T**` that
  preserves dynamic type via `clone()`).
- `BitSet` — compact set of non-negative integers; `|=`, `&=`, `%=` are
  union / intersection / set-difference (note: `%=` is *difference*, not modulo).
- `Interval` — closed $[a, b]$ with union and intersection.
- `Integral` (abstract) + `TrapezoidalRuleIntegral` — numerical integration
  with $n$ sub-intervals (default $10^6$, see `Constants.h`).
- `special_functions/Gamma` — $\Gamma(x)$ and $\ln\Gamma(x)$ via the Lanczos
  approximation with reflection formula for $x < 0.5$.
- `combinatorics/` — `Factoriel` and `KSelection` (combinations, variations,
  permutations, with/without repetition).

### 2. `source/events/` and `source/sigma_algebra/`
- `ElementaryEvent` — atomic outcome with auto-generated unique id.
- `Event` — subset of $\Omega$, backed by both a `Vector<ElementaryEvent>` and
  a `BitSet` for $O(1)$ id-membership tests. `|` and `&` are real set
  operations.
- `Omega` — distinguished event representing the whole sample space.
- `FullGroupOfEvents` — a partition of $\Omega$ (hypotheses).
- `SigmaAlgebra` + `SigmaAlgebraFactory` — generates a $\sigma$-algebra from an
  `Omega` using a `SigmaAlgebraPattern` (`Trivial` $\{\emptyset, \Omega\}$ or
  `PowerSet` $2^\Omega$, capped at `POWER_SET_MAX_ELEMENTS = 20`).
- `ProbabilitySpace` — the triple $(\Omega, \Sigma, P)$.

### 3. `source/functions/`
A templated callable hierarchy rooted at `Function<T, U>`:
- `ProbabilityFunction : Function<Event, double>` — probability measure
  $P: \Sigma \to [0,1]$, stored as a `Vector<Pair<Event, double>>`.
- `ConditionalProbabilityFunction` — extends the above with
  $P(A\mid B) = P(A \cap B) / P(B)$.
- `FullProbabilityFormula` — composes both into
  $P(A) = \sum_i P(A \mid H_i)\, P(H_i)$.
- `DensityFunction : Function<double, double>` and its specialisations:
  `UniformDensityFunction`, `NormalDensityFunction`,
  `ExponentialDensityFunction`, `GammaDensityFunction`,
  `ChiSquaredDensityFunction`, `StudentTDensityFunction`. Heavy PDFs use a
  log-trick through `logGamma` for numerical stability.

### 4. `source/distributions/`
Random variables built on top of the function layer.

`RandomVariable<T>` exposes `calculateProbability`, `getExpectation`,
`getVariance`, `getOverallType`. It splits into:

| Discrete (`DiscreteRandomVariable<T>`) | Continuous (`ContinuousRandomVariable<Interval>`) |
| --- | --- |
| `Bernoulli(p)` — $P(X=1)=p$ | `Uniform(a,b)` — $f(x) = \frac{1}{b-a}$ |
| `Binomial(n,p)` — $P(X=k)=\binom{n}{k}p^k(1-p)^{n-k}$ | `Normal(\mu,\sigma^2)` — $f(x)=\frac{1}{\sigma\sqrt{2\pi}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$ |
| `Geometric(p)` — $P(X=k)=p(1-p)^k$ | `Exponential(\lambda)` — $f(x)=\lambda e^{-\lambda x}$ |
| `NegativeBinomial(r,p)` — $P(X=k)=\binom{k+r-1}{r-1}p^r(1-p)^k$ | `Gamma(\alpha,\beta)` — $f(x)=\frac{\beta^\alpha}{\Gamma(\alpha)}x^{\alpha-1}e^{-\beta x}$ |
| `Poisson(\lambda)` — $P(X=k)=\frac{\lambda^k e^{-\lambda}}{k!}$ | `ChiSquared(k)` — $f(x)=\frac{1}{2^{k/2}\Gamma(k/2)}x^{k/2-1}e^{-x/2}$ |
| `HyperGeometric(N,K,n)` — $P(X=k)=\frac{\binom{K}{k}\binom{N-K}{n-k}}{\binom{N}{n}}$ | `StudentT(\nu)` — $f(t)\propto\left(1+\frac{t^2}{\nu}\right)^{-(\nu+1)/2}$ |

Bernoulli, Binomial, Geometric, and NegativeBinomial share the
`BernoulliSchemeRandomVariable<T>` base. Continuous distributions evaluate
$P(X \in [a,b])$ by trapezoidal integration of their density; unbounded
supports are truncated using the cutoff multipliers in `Constants.h`
(`NORMAL_SIGMA_CUTOFF_MULTIPLIER`, `EXPONENTIAL_CUTOFF_MULTIPLIER`, …).

**Joint distributions** live in `distributions/joint_distributions/`:
- `JointDiscreteDistribution<T>` — marginals + per-variable supports + either
  an explicit PMF table or a functor. Exposes `jointProbability`, `marginal`,
  `conditional`, `covariance`, `correlation`, `areIndependent`.
- `JointContinuousDistribution` — marginals + supports + PDF functor. Uses
  tensor trapezoidal integration over each dimension.

The mode of construction is captured by `JointDistributionMode`
(`Independent` / `TableGeneral` / `FunctorGeneral`).

### 5. `source/independence/`
- `EventsIndependence` — pairwise: $|P(A \cap B) - P(A)P(B)| \le \varepsilon$.
- `TotalityEventsIndependence` — mutual independence over $k$ events;
  enumerates all non-trivial subsets via bitmasks and verifies
  $P\bigl(\bigcap_{i \in S} A_i\bigr) = \prod_{i \in S} P(A_i)$.

## Tuning

All numerical knobs live in `source/Constants.h`:

| Constant | Purpose |
| --- | --- |
| `EPS`, `INDEPENDENCE_CHECK_TOLERANCE` | Floating-point comparison tolerances |
| `COUNT_OF_SUB_INTERVALS` | Trapezoidal rule resolution ($10^6$) |
| `POWER_SET_MAX_ELEMENTS`, `SIGMA_ALGEBRA_SIZE_LIMIT` | Combinatorial guards |
| `*_CUTOFF_MULTIPLIER` | How far past the mean to truncate unbounded supports |
| `JOINT_CONTINUOUS_STEPS_PER_DIMENSION` | Resolution of tensor integration |

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
