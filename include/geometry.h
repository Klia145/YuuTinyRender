#pragma once
#include "vec3.h"
#include "mat4.h"
#include <cmath>
#include "vec2.h"
#include "vec4.h"


int cross(int dx, int dy, int ex, int ey);
float cross(float dx, float dy, float ex, float ey);
vec3 normalize(vec3 v);

mat4 TransformModel_To_View(const float& angle_radians);