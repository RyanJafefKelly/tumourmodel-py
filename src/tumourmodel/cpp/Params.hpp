#pragma once
#include <random>
#include <cstdint>

inline std::uniform_real_distribution<double> uniform_random(0.0, 1.0);

class Params {
private:
    std::mt19937 generator;

public:
    int gage, page;
    double p_0, dmax, p_psc;

    // Original behavior (non-deterministic RNG)
    Params(double p0, double psc, double dmax_, int gage_, int page_)
    : generator(std::random_device{}()), gage(gage_), page(page_), p_0(p0), dmax(dmax_), p_psc(psc) {}

    // New deterministic RNG
    Params(double p0, double psc, double dmax_, int gage_, int page_, std::uint64_t seed)
    : generator(static_cast<std::mt19937::result_type>(seed)), gage(gage_), page(page_), p_0(p0), dmax(dmax_), p_psc(psc) {}

    double RandomDouble() { return uniform_random(generator); }
    bool WithProbability(double prob) { return RandomDouble() < prob; }

    static const double s;
    static const double mu;
    static const double Delta_t;
    static const double rmin;
    static const int tinterval;
};
