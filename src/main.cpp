
#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"
#include <memory>

static hittable_list list;

void add_sphere(const point3 &loc, double r, std::shared_ptr<material> mat) {
  list.add(std::make_shared<sphere>(loc, r, std::move(mat)));
}

void setup_scene() {
  color GOLD(1.0, 0.78, 0.34);
  add_sphere(point3(0, 0, -1), 0.5, std::make_shared<metal>(GOLD));

  color RED(0.7, 0.3, 0.3);
  add_sphere(point3(0.25, 0.3, -0.5), 0.15, std::make_shared<metal>(RED));

  color GREEN(0.2, 0.9, 0.1);
  add_sphere(point3(0, -0.3, 0.5), 0.3, std::make_shared<metal>(GREEN, 0.1));

  add_sphere(point3(0, -100.5, 0), 100, std::make_shared<metal>());

  add_sphere(point3(-0.1, -0.1, -0.4), 0.1, std::make_shared<dielectric>(1.4));
  add_sphere(point3(-0.1, -0.1, -0.4), -0.09,
             std::make_shared<dielectric>(1.4));

  color TAN(0.82, 0.71, 0.55);
  add_sphere(point3(-6, 0.2, -7), 1, std::make_shared<lambertian>(TAN));
}

int main() {
  setup_scene();

  camera cam;
  cam.aspect_ratio = 931.0 / 556.0;
  cam.image_width = 931;
  cam.samples_per_pixel = 512;
  cam.max_depth = 10;

  cam.vfov = 90;
  cam.lookfrom = point3(0, 0, 0);
  cam.lookat = point3(0, 0, -1);
  cam.vup = vec3(0, 1, 0);

  cam.render(list);
}
