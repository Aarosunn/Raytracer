
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
  metal() : albedo(color(0.5, 0.5, 0.5)), fuzz_(0.015) {}

  metal(const color &c) : albedo(c), fuzz_(0.015) {}

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

class dielectric : public material {
  double refractive_;

public:
  dielectric(double refractive) : refractive_(refractive) {}

  bool scatter(const ray &r, const record &rc, color &attenuation,
               ray &scattered) const override {
    double schlick = std::pow((1 - refractive_) / (1 + refractive_), 2);
    double reflectance =
        schlick +
        (1 - schlick) *
            std::pow(1 + dot(unit_vector(r.direction()), rc.normal), 5);
    vec3 direction_in;

    if (random_double(0, 1) < reflectance)
      direction_in = unit_vector(reflect(r.direction(), rc.normal));
    else
      direction_in = refract(unit_vector(r.direction()), rc.normal,
                             rc.front_face ? 1.0 / refractive_ : refractive_);

    scattered = ray(rc.point, direction_in);
    attenuation = color(1, 1, 1);
    return true;
  }
};
