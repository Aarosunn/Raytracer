
#pragma once

#include "hittable.h"
#include "random.h"
#include "ray.h"
#include <algorithm>
#include <cmath>

class material {
public:
  virtual ~material() = default;
  virtual bool scatter(const ray &r, const record &rc, color &attenuation,
                       ray &scatter) const = 0;
};

class lambertian : public material {
  color albedo;

public:
  lambertian() : albedo(color(0.5, 0.5, 0.5)) {}

  lambertian(const color &c) : albedo(c) {}

  bool scatter(const ray &, const record &rc, color &attenuation,
               ray &scattered) const override {
    scattered = ray(rc.point, random_unit_vector() + rc.normal);
    attenuation = albedo;
    return true;
  }
};

class metal : public material {
  color albedo;
  double fuzz_;

public:
  metal() : albedo(color(0.5, 0.5, 0.5)), fuzz_(0.2) {}

  metal(const color &c) : albedo(c), fuzz_(0.2) {}

  metal(const color &c, const double fuzz)
      : albedo(c), fuzz_(std::clamp(fuzz, 0.0, 1.0)) {}

  bool scatter(const ray &r, const record &rc, color &attenuation,
               ray &scattered) const override {
    scattered = ray(rc.point, unit_vector(reflect(r.direction(), rc.normal)) +
                                  fuzz_ * random_unit_vector());
    attenuation =
        albedo +
        (color(1, 1, 1) - albedo) *
            std::pow(1 + dot(unit_vector(r.direction()), rc.normal), 5);
    return (dot(scattered.direction(), rc.normal) > 0);
  }
};
