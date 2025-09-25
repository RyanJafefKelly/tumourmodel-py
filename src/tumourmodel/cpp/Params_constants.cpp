#include "Params.hpp"

// Mechanical and timing constants (original TumourModel values)
const double Params::s        = 3.03;  // spring rest length
const double Params::mu       = 0.01;  // motility coefficient
const double Params::Delta_t  = 60;    // seconds per integration step
const double Params::rmin     = 2.0;   // crowding threshold (distance)
const int    Params::tinterval = 24;   // hours per simulated day
