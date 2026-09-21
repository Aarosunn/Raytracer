
#pragma once

#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <ostream>

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 900;
constexpr int image_height =
    std::max(1, static_cast<int>(image_width / aspect_ratio));

const point3 camera_center{0, 0, 0};
const double focal_length{1.0};
const double viewport_height{2.0};
const double viewport_width{viewport_height *
                            (static_cast<double>(image_width) / image_height)};

const vec3 viewport_u{viewport_width, 0, 0};
const vec3 viewport_v{0, -viewport_height, 0};

const vec3 pixel_delta_u{viewport_u / image_width};
const vec3 pixel_delta_v{viewport_v / image_height};

const vec3 viewport_upper_left{camera_center - vec3(0, 0, focal_length) -
                               viewport_u / 2 - viewport_v / 2};
const point3 pixel00_loc{viewport_upper_left +
                         0.5 * (pixel_delta_u + pixel_delta_v)};

const point3 sphere_center(0, 0, -1);
const double sphere_radius{0.5};

struct record {
  double t;
  point3 point;
  vec3 normal;
};

std::ostream &write_color(std::ostream &out, const color &c);

color ray_color(const ray &r);

class hittable {
public:
  virtual ~hittable() = default;
  virtual bool check_hit(const ray &r, const double t_min, const double t_max,
                         record &rc) const = 0;
};

class sphere : public hittable {
  point3 center_;
  double radius_;

public:
  sphere(const point3 &center, double radius)
      : center_(center), radius_(radius) {}

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
    return true;
  }
};
