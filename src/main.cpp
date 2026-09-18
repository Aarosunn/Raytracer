
#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <iostream>

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 256;
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
      point3 pixel_center = pixel00_loc + j * pixel_delta_u + i * pixel_delta_v;
      vec3 ray_direction = pixel_center - camera_center;
      write_color(std::cout, ray_color(ray(camera_center, ray_direction)));
      std::cout << "   ";
    }
    std::cerr << image_height - i << " rows left\n";
    std::cout << '\n';
  }
}
