
#pragma once

#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>

inline int to_byte(double c) {
  return static_cast<int>(std::sqrt(std::clamp(c, 0.0, 1.0)) * 255.999);
}

inline std::ostream &write_color(std::ostream &out, const color &c) {
  out << to_byte(c.x()) << " " << to_byte(c.y()) << " " << to_byte(c.z());
  return out;
}

class camera {
public:
  double aspect_ratio{16.0 / 9.0};
  int image_width{400};
  int samples_per_pixel{32};
  int max_depth{10};

  double vfov{90};
  point3 lookfrom{0, 0, 0};
  point3 lookat{0, 0, -1};
  vec3 vup{0, 1, 0};

  void render(const hittable &world) {
    initialize();

    std::cout << "P3\n"
              << image_width << " " << image_height_ << '\n'
              << "255\n";

    for (int i = 0; i < image_height_; ++i) {
      std::cerr << image_height_ - i << " rows left\n";
      for (int j = 0; j < image_width; ++j) {
        color color_acc{};
        for (int n = 0; n < samples_per_pixel; ++n)
          color_acc += ray_color(get_ray(i, j), world, 0);

        write_color(std::cout, color_acc / samples_per_pixel);
        std::cout << "   ";
      }
      std::cout << '\n';
    }
  }

private:
  int image_height_;
  point3 center_;
  point3 pixel00_loc_;
  vec3 pixel_delta_u_;
  vec3 pixel_delta_v_;
  vec3 cam_u_, cam_v_, cam_w_;

  static constexpr double t_min = 0.001;
  static constexpr double t_max = std::numeric_limits<double>::infinity();

  void initialize() {
    image_height_ = std::max(1, static_cast<int>(image_width / aspect_ratio));
    center_ = lookfrom;

    double focal_length{(lookfrom - lookat).length()};
    double viewport_height{2 * focal_length *
                           std::tan((vfov / 2) * (std::numbers::pi / 180))};
    double viewport_width{viewport_height *
                          (static_cast<double>(image_width) / image_height_)};
    cam_w_ = unit_vector(lookfrom - lookat);
    cam_u_ = unit_vector(cross(vup, cam_w_));
    cam_v_ = cross(cam_w_, cam_u_);

    vec3 viewport_u{viewport_width * cam_u_};
    vec3 viewport_v{viewport_height * -cam_v_};

    pixel_delta_u_ = viewport_u / image_width;
    pixel_delta_v_ = viewport_v / image_height_;

    vec3 viewport_upper_left{center_ - focal_length * cam_w_ - viewport_u / 2 -
                             viewport_v / 2};
    pixel00_loc_ =
        viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
  }

  ray get_ray(int i, int j) const {
    point3 pixel_center{pixel00_loc_ + j * pixel_delta_u_ + i * pixel_delta_v_};
    point3 jitter_pixel{pixel_center +
                        (random_double() - 0.5) * pixel_delta_u_ +
                        (random_double() - 0.5) * pixel_delta_v_};
    return ray(center_, jitter_pixel - center_);
  }

  color ray_color(const ray &r, const hittable &scene, int depth) const {
    if (depth > max_depth)
      return color(0, 0, 0);

    record rc{};
    if (scene.check_hit(r, t_min, t_max, rc)) {
      ray scattered;
      color attenuation;
      if (rc.mat->scatter(r, rc, attenuation, scattered))
        return attenuation * ray_color(scattered, scene, ++depth);
      return color(0, 0, 0);
    }

    vec3 normalized = unit_vector(r.direction());
    double remap{(normalized.y() + 1) / 2};
    const color start{1.0, 1.0, 1.0};
    const color end{0.5, 0.7, 1.0};
    return (1 - remap) * start + remap * end;
  }
};
