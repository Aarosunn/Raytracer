
#include "main.h"
#include "hittable.h"
#include "sphere.h"
#include <iostream>
#include <limits>
#include <memory>

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
  std::cout << "P3\n" << image_width << " " << image_height << '\n' << "255\n";
  hittable_list list;
  list.add(std::make_shared<sphere>(point3(0, 0, -1), 1.1));
  list.add(std::make_shared<sphere>(point3(0.3, 0.3, -0.8), 0.2));
  list.add(std::make_shared<sphere>(point3(-0.3, -0.3, -0.8), 0.2));

  const double t_min = 0;
  const double t_max = std::numeric_limits<double>::infinity();
  record rc{};

  for (int i = 0; i < image_height; ++i) {
    for (int j = 0; j < image_width; ++j) {

      point3 pixel_center{pixel00_loc + j * pixel_delta_u + i * pixel_delta_v};
      vec3 ray_direction{pixel_center - camera_center};

      ray r(camera_center, ray_direction);

      if (list.check_hit(r, t_min, t_max, rc)) {
        write_color(std::cout, (rc.normal + vec3(1, 1, 1)) / 2);
      } else
        write_color(std::cout, ray_color(r));
      std::cout << "   ";
    }
    std::cerr << image_height - i << " rows left\n";
    std::cout << '\n';
  }
}
