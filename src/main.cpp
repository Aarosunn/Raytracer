
#include "main.h"
#include "hittable.h"
#include "material.h"
#include "random.h"
#include "sphere.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

constexpr double t_min = 0.001;
constexpr double t_max = std::numeric_limits<double>::infinity();
constexpr int N = 32;
constexpr int depth_limit = 10;

int to_byte(double c) {
  return static_cast<int>(std::sqrt(std::clamp(c, 0.0, 1.0)) * 255.999);
}

std::ostream &write_color(std::ostream &out, const color &c) {
  out << to_byte(c.x()) << " " << to_byte(c.y()) << " " << to_byte(c.z());
  return out;
}

color ray_color(const ray &r, const hittable &scene, int depth) {
  if (depth > depth_limit)
    return color(0, 0, 0);

  record rc{};
  if (scene.check_hit(r, t_min, t_max, rc)) {
    ray scattered;
    color attenuation;
    if (rc.mat->scatter(r, rc, attenuation, scattered))
      return attenuation * ray_color(scattered, scene, ++depth);
    else
      return color(0, 0, 0);
  }

  vec3 normalized = unit_vector(r.direction());
  double remap{(normalized.y() + 1) / 2};
  const color start{1.0, 1.0, 1.0};
  const color end{0.5, 0.7, 1.0};
  return (1 - remap) * start + remap * end;
}

int main() {
  std::cout << "P3\n" << image_width << " " << image_height << '\n' << "255\n";
  hittable_list list;
  list.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5,
                                    std::make_shared<metal>()));
  list.add(std::make_shared<sphere>(
      point3(0.25, 0.3, -0.5), 0.15,
      std::make_shared<lambertian>(color(0.7, 0.3, 0.3))));
  list.add(std::make_shared<sphere>(
      point3(0, -0.3, 0.5), 0.3,
      std::make_shared<lambertian>(color(0.2, 0.9, 0.1))));

  for (int i = 0; i < image_height; ++i) {
    for (int j = 0; j < image_width; ++j) {

      point3 pixel_center{pixel00_loc + j * pixel_delta_u + i * pixel_delta_v};
      color color_acc{};
      for (int n = 0; n < N; ++n) {
        point3 jitter_pixel{pixel_center +
                            (random_double() - 0.5) * pixel_delta_u +
                            (random_double() - 0.5) * pixel_delta_v};
        vec3 ray_direction{jitter_pixel - camera_center};
        ray r(camera_center, ray_direction);
        color_acc += ray_color(r, list, 0);
      }

      color color_avg = color_acc / N;
      write_color(std::cout, color_avg);
      std::cout << "   ";
    }
    std::cerr << image_height - i << " rows left\n";
    std::cout << '\n';
  }
}
