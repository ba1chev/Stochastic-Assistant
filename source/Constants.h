#pragma once

#include <cstddef>

static const char* RED = "\x1b[31m";
static const char* RESET = "\x1b[0m";

static constexpr double EPS = 1e-9;
static constexpr int COUNT_OF_ELEMENTS_IN_ONE_BUCKET = 8;
static constexpr int MAX_TOLERANCE_BITSET = 10000;
static constexpr double EULER_NUMBER = 2.71828182846;
static constexpr double PI = 3.14159265359;
static constexpr int COUNT_OF_SUB_INTERVALS = 1000000;

static constexpr double JOINT_DISCRETE_PMF_SUM_TOLERANCE = 1e-6;
static constexpr double INDEPENDENCE_CHECK_TOLERANCE = 1e-3;
static constexpr size_t JOINT_CONTINUOUS_STEPS_PER_DIMENSION = 64;
static constexpr size_t INDEPENDENCE_SAMPLES_PER_DIMENSION = 4;

static constexpr double NORMAL_SIGMA_CUTOFF_MULTIPLIER = 10.0;
static constexpr double EXPONENTIAL_CUTOFF_MULTIPLIER = 50.0;
static constexpr double STUDENT_T_SIGMA_CUTOFF_MULTIPLIER = 10.0;
static constexpr double STUDENT_T_SMALL_DF_CUTOFF = 50.0;
static constexpr double CHI_SQUARED_SIGMA_CUTOFF_MULTIPLIER = 10.0;

static constexpr size_t POWER_SET_MAX_ELEMENTS = 20;
static constexpr size_t SIGMA_ALGEBRA_SIZE_LIMIT = 32;
static constexpr size_t DEFAULT_CONTAINER_CAPACITY = 8;

static const char* ERR_SUPPORTS_SIZE_MISMATCH = "Supports size must match marginals size";
static const char* ERR_JOINT_REQUIRES_MARGINAL = "Joint distribution requires at least one marginal";
static const char* ERR_INDEX_OUT_OF_RANGE = "Index is out of range";
static const char* ERR_CORRELATION_ZERO_VARIANCE = "Correlation undefined: variance is zero";
