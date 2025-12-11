#include<cmath>
#include<iostream>
#include"vec3.h"
vec3 vec3::operator+(const vec3&v)const{
    return vec3(x+v.x,y+v.y,z+v.z);
}
vec3 vec3::operator-(const vec3&v)const{
    return vec3(x-v.x,y-v.y,z-v.z);
}
vec3 vec3::operator*(float t)const{
    return vec3(x*t,y*t,z*t);
}
float vec3::operator*(const vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}
vec3 vec3::cross(const vec3& v) const {
    return vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}
float vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize() const {
    float len = length();
    if (len == 0) {
        return *this;
    }
    return *this * (1.0f / len);
}

// friend 函数不需要 vec3:: 前缀
std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
float vec3::dot(const vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

bool vec3::operator==(const vec3&v )const{
    static const float EPSILON=1e-6f;
    return std::abs(x - v.x) < EPSILON &&
           std::abs(y - v.y) < EPSILON &&
           std::abs(z - v.z) < EPSILON;   

    
}
vec3& vec3::operator*=(float n){
    x*=n;
    y*=n;
    z*=n;
    return *this;
}
vec3& vec3::operator/=(float n){
    static const float EPSILON=1e-6f;
    if(std::abs(n)<EPSILON){
        n=EPSILON;
    }
    x/=n;
    y/=n;
    z/=n;
    return *this;
}