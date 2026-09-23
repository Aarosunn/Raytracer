
#include "main.h"
#include "hittable.h"
#include "sphere.h"
#include <iostream>
#include <limits>
#include <memory>
#include <random>

static std::mt19937 gen(42);
static std::uniform_real_distribution<double> dist(0.0, 1.0);

constexpr double t_min = 0.001;
constexpr double t_max = std::numeric_limits<double>::infinity();
constexpr int N = 32;
constexpr int depth_limit = 10;

double random_double() { return dist(gen); }

double random_double(double min, double max) {
  return (random_double() * (max - min)) + min;
}

vec3 random_unit_vector() {
  vec3 rv;
  do {
    rv = vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
  } while (rv.length_squared() > 1 || rv.length_squared() < 1e-160);
  return rv / rv.length();
}

std::ostream &write_color(std::ostream &out, const color &c) {
  out << static_cast<int>(c.x() * 255.999) << " "
      << static_cast<int>(c.y() * 255.999) << " "
      << static_cast<int>(c.z() * 255.999);
  return out;
}

color ray_color(const ray &r, const hittable &scene, int depth) {
  if (depth > depth_limit)
    return color(0, 0, 0);

  record rc{};
  if (scene.check_hit(r, t_min, t_max, rc)) {
    ray bounce(rc.point, random_unit_vector() + rc.normal);
    return 0.5 * ray_color(bounce, scene, ++depth);
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
  list.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  list.add(std::make_shared<sphere>(point3(0.3, 0.3, -0.8), 0.2));
  list.add(std::make_shared<sphere>(point3(-0.3, -0.3, -0.8), 0.2));

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
