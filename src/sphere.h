
#pragma once

#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include <cmath>
#include <memory>

class sphere : public hittable {
  point3 center_;
  double radius_;
  std::shared_ptr<material> mat_;

public:
  sphere(const point3 &center, double radius, std::shared_ptr<material> mat)
      : center_(center), radius_(radius), mat_(std::move(mat)) {}

  bool check_hit(const ray &r, const double t_min, const double t_max,
                 record &rc) const override {
    vec3 oc{r.origin() - center_};
    double a{dot(r.direction(), r.direction())};
    double h{dot(r.direction(), oc)};
    double c{dot(oc, oc) - radius_ * radius_};
    double disc{h * h - a * c};

    if (disc < 0)
      return false;

    double discroot{std::sqrt(disc)};
    double t_far{(-h + discroot) / a};
    double t_near{(-h - discroot) / a};
    double t_survive{t_near};

    if (t_survive < t_min || t_survive > t_max)
      t_survive = t_far;
    if (t_survive < t_min || t_survive > t_max)
      return false;

    rc.t = t_survive;
    rc.point = r.at(t_survive);
    rc.normal = ((rc.point - center_) / radius_);
    rc.mat = mat_;
    rc.set_face_normal(r);
    return true;
  }
};
