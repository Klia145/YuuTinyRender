#include "vec4.h"
#include <cmath>

// 向量加法
vec4 vec4::operator+(const vec4& v) const {
    return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

// 向量减法
vec4 vec4::operator-(const vec4& v) const {
    return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

// 标量乘法
vec4 vec4::operator*(float t) const {
    return vec4(x * t, y * t, z * t, w * t);
}

// 点积运算
float vec4::operator*(const vec4& v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

// 叉积运算（只对前三个分量进行计算）
vec4 vec4::cross(const vec4& v) const {
    return vec4(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x,
        0
    );
}

// 计算向量长度
float vec4::length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

// 向量归一化
vec4 vec4::normalize() const {
    float len = length();
    if (len == 0) return vec4(0, 0, 0, 0);
    return vec4(x / len, y / len, z / len, w / len);
}

// 点积运算（另一种实现方式）
float vec4::dot(const vec4& v) const {
    return *this * v;
}

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const vec4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}
