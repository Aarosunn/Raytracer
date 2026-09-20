
#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <iostream>
#include <optional>

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 900;
constexpr int image_height =
    std::max(1, static_cast<int>(image_width / aspect_ratio));

std::ostream &write_color(std::ostream &out, const color &c) {
  out << static_cast<int>(c.x() * 255.999) << " "
      << static_cast<int>(c.y() * 255.999) << " "
      << static_cast<int>(c.z() * 255.999);
  return out;
}

color ray_color(const ray &r) {
  vec3 normalize = unit_vector(r.direction());
  double remap{(normalize.y() + 1) / 2};
  const color start{1.0, 1.0, 1.0};
  const color end{0.5, 0.7, 1.0};
  color result{(1 - remap) * start + remap * end};
  return result;
}

std::optional<double> solve_quadratic(double a, double h, double disc) {
  double discroot = std::sqrt(disc);
  double t1{(-h + discroot) / a};
  double t2{(-h - discroot) / a};
  if (t2 >= 0)
    return t2;
  if (t1 < 0)
    return std::nullopt;
  else
    return t1;
}

std::optional<double> hit_sphere(const point3 &center, double radius,
                                 const ray &r) {
  vec3 oc{r.origin() - center};
  double a{dot(r.direction(), r.direction())};
  double h = dot(r.direction(), oc);
  double c{dot(oc, oc) - radius * radius};
  double disc = h * h - a * c;
  if (disc < 0)
    return std::nullopt;
  else
    return solve_quadratic(a, h, disc);
}

int main() {
  const point3 camera_center{0, 0, 0};
  const double focal_length{1.0};
  const double viewport_height{2.0};
  const double viewport_width{
      viewport_height * (static_cast<double>(image_width) / image_height)};

  const vec3 viewport_u{viewport_width, 0, 0};
  const vec3 viewport_v{0, -viewport_height, 0};

  const vec3 pixel_delta_u{viewport_u / image_width};
  const vec3 pixel_delta_v{viewport_v / image_height};

  const vec3 viewport_upper_left{camera_center - vec3(0, 0, focal_length) -
                                 viewport_u / 2 - viewport_v / 2};
  const point3 pixel00_loc{viewport_upper_left +
                           0.5 * (pixel_delta_u + pixel_delta_v)};

  std::cout << "P3\n" << image_width << " " << image_height << '\n' << "255\n";
  for (int i = 0; i < image_height; ++i) {
    for (int j = 0; j < image_width; ++j) {

      point3 pixel_center{pixel00_loc + j * pixel_delta_u + i * pixel_delta_v};
      vec3 ray_direction{pixel_center - camera_center};

      ray r(camera_center, ray_direction);

      point3 sphere_center(0, 0, -1);
      double sphere_radius = 0.5;

      if (auto t = hit_sphere(sphere_center, sphere_radius, r)) {
        vec3 normalize((r.at(t.value()) - sphere_center) / sphere_radius);
        normalize = ((normalize + vec3(1, 1, 1)) / 2.0);
        write_color(std::cout, normalize);
      } else
        write_color(std::cout, ray_color(r));
      std::cout << "   ";
    }
    std::cerr << image_height - i << " rows left\n";
    std::cout << '\n';
  }
}
