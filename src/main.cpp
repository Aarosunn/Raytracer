
#include <iostream>

constexpr int image_width = 256;
constexpr int image_height = 256;

int main() {
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
