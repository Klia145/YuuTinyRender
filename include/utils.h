#pragma once
#include "vec4.h"
#include <tuple>
// 投影函数声明
std::tuple<int, int, float> project(vec4 clip_pos);
vec3 normalize(const vec3 v1);