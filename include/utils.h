#pragma once
#include "vec4.h"
#include <tuple>
#include<algorithm>
// 投影函数声明
std::tuple<int, int, float> project(vec4 clip_pos);
inline float lerp(float a,float b,float t){
    return a*(1.0f-t)+b*t;
}

inline unsigned char lerp_byte(unsigned char a ,unsigned char b,float t){
    return (unsigned char)(a*(1.0f-t)+b*t);
}
template<typename T>
inline T clamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}
