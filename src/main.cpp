
#include "vec3.h"
#include <iostream>

constexpr int image_width = 256;
constexpr int image_height = 256;

void output() {
  std::cout << "P3\n" << image_width << " " << image_height << '\n' << "255\n";
  for (int i = 0; i < image_height; ++i) {
    for (int j = 0; j < image_width; ++j) {
      std::cout << j << " " << 0 << " " << i << "   ";
    }
    std::cout << '\n';
  }
  // std::cout << "P3\n"
  //           << "3 2\n"
  //           << "255\n"
  //           << "255 0 0   0 255 0   0 0 255\n"
  //           << "255 255 0   255 255 255   0 0 0\n";
}

int main() {
  vec3 v1(1, 2, 3);
  vec3 v2(4, 5, 6);

  std::cout << v1 << '\n' << v2 << '\n';
  std::cout << v1 + v2 << '\n';
  std::cout << v2 - v1 << '\n';
  std::cout << dot(v1, v2) << '\n';
  std::cout << cross(v1, v2) << '\n';

  std::cout << v1 / 2 << '\n';
  std::cout << 3 * v2 << '\n';

  std::cout << v2.length() << '\n';
  v2 += v1;

  std::cout << v2 << '\n';
  std::cout << v2.length() << '\n';
  std::cout << unit_vector(v2) << '\n';
}
