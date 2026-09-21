
#pragma once

#include "ray.h"
#include <memory>
#include <vector>

struct record {
  point3 point;
  vec3 normal;
  double t;
  bool front_face;

  void set_face_normal(const ray &r) {
    if (dot(r.direction(), normal) > 0) {
      normal = -normal;
      front_face = false;
    } else {
      front_face = true;
    }
  }
};

class hittable {
public:
  virtual ~hittable() = default;
  virtual bool check_hit(const ray &r, const double t_min, const double t_max,
                         record &rc) const = 0;
};

class hittable_list : public hittable {
  std::vector<std::shared_ptr<hittable>> list;

public:
  hittable_list() = default;

  hittable_list(std::shared_ptr<hittable> h) { add(std::move(h)); }

  void add(std::shared_ptr<hittable> h) { list.push_back(std::move(h)); }

  void clear() { list.clear(); }

  bool check_hit(const ray &r, const double t_min, const double t_max,
                 record &rc) const override {

    bool hit_something = false;
    double running_t = t_max;
    for (const auto &l : list) {
      if (l->check_hit(r, t_min, running_t, rc)) {
        hit_something = true;
        running_t = rc.t;
      }
    }
    return hit_something;
  }
};
