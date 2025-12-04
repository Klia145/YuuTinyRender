#include "geometry.h"
#include<cmath>
int cross(int dx, int dy, int ex, int ey) {
    return dx * ey - dy * ex;
}

float cross(float dx, float dy, float ex, float ey) {
    return dx * ey - dy * ex;
}

vec3 normalize(vec3 v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return vec3(v.x / len, v.y / len, v.z / len);
}

mat4 TransformModel_To_View(const float& angle_radians) {
    mat4 model_mat = mat4::rotate_y(angle_radians);
    mat4 view = mat4::translate(0, 0, -3);
    mat4 proj = mat4::identity();
    mat4 mvp = proj * view * model_mat;
    return mvp;
}