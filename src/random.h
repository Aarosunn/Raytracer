
#pragma once

#include "vec3.h"
#include <random>

inline std::mt19937 gen(42);
inline std::uniform_real_distribution<double> dist(0.0, 1.0);

inline double random_double() { return dist(gen); }

inline double random_double(double min, double max) {
  return (random_double() * (max - min)) + min;
}

inline vec3 random_unit_vector() {
  vec3 rv;
  do {
    rv = vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
  } while (rv.length_squared() > 1 || rv.length_squared() < 1e-160);
  return rv / rv.length();
}
