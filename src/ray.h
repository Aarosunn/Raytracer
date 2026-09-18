
#pragma once
#include "vec3.h"

class ray {
  point3 origin_;
  vec3 direction_;

public:
  ray() {}

  ray(const vec3 &origin_in, const vec3 &direction_in)
      : origin_(origin_in), direction_(direction_in) {}

  const point3 &origin() const { return origin_; }
  const vec3 &direction() const { return direction_; }

  point3 at(double t) const { return origin_ + t * direction_; }
};
