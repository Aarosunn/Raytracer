
#pragma once

#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <ostream>

constexpr double aspect_ratio = 931.0 / 556.0;
constexpr int image_width = 931;
constexpr int image_height =
    std::max(1, static_cast<int>(image_width / aspect_ratio));

const point3 camera_center{0, 0, 0};
const double focal_length{1.0};
const double viewport_height{2.0};
const double viewport_width{viewport_height *
                            (static_cast<double>(image_width) / image_height)};

const vec3 viewport_u{viewport_width, 0, 0};
const vec3 viewport_v{0, -viewport_height, 0};

const vec3 pixel_delta_u{viewport_u / image_width};
const vec3 pixel_delta_v{viewport_v / image_height};

const vec3 viewport_upper_left{camera_center - vec3(0, 0, focal_length) -
                               viewport_u / 2 - viewport_v / 2};
const point3 pixel00_loc{viewport_upper_left +
                         0.5 * (pixel_delta_u + pixel_delta_v)};
