
#pragma once
#include <cmath>
#include <ostream>

class vec3 {
  double x_, y_, z_;

public:
  vec3() : x_(0), y_(0), z_(0) {}

  vec3(double x_in, double y_in, double z_in) : x_(x_in), y_(y_in), z_(z_in) {}

  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

  vec3 operator-() const { return vec3(-x_, -y_, -z_); }
  vec3 &operator+=(const vec3 &v) {
    x_ += v.x_;
    y_ += v.y_;
    z_ += v.z_;

    return *this;
  }
  vec3 &operator*=(double t) {
    x_ *= t;
    y_ *= t;
    z_ *= t;

    return *this;
  }
  vec3 &operator/=(double t) { return operator*=(1.0 / t); }

  double length_squared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
  double length() const { return std::sqrt(length_squared()); }
};

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
  return vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
  return vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
  return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(v.x() * t, v.y() * t, v.z() * t);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
  return vec3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

inline vec3 operator/(const vec3 &v, double t) { return (1.0 / t) * v; }

inline double dot(const vec3 &v1, const vec3 &v2) {
  return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
  return vec3(v1.y() * v2.z() - v1.z() * v2.y(),
              -(v1.x() * v2.z() - v1.z() * v2.x()),
              v1.x() * v2.y() - v1.y() * v2.x());
}

inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  out << v.x() << " " << v.y() << " " << v.z();
  return out;
}

using point3 = vec3;
using color = vec3;
