
#pragma once

#include "hittable.h"
#include "random.h"
#include "ray.h"

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

public:
  metal() : albedo(color(0.5, 0.5, 0.5)) {}

  metal(const color &c) : albedo(c) {}

  bool scatter(const ray &r, const record &rc, color &attenuation,
               ray &scattered) const override {
    scattered = ray(rc.point, reflect(r.direction(), rc.normal));
    attenuation = albedo;
    return true;
  }
};
