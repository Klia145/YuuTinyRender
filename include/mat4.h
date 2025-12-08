#pragma once
#include "vec4.h"

class mat4{
    public:
    float m[4][4];
    mat4();
    mat4(float diagonal);

    mat4 operator*(const mat4& other)const;
    vec4 operator*(const vec4&v)const;
    friend vec4 operator*(const vec4&v,const mat4&m);
    static mat4 identity();
    static mat4 translate(float x,float y,float z);
    static mat4 scale(float x,float y,float z);
    static mat4 rotate_x(float angle);
    static mat4 rotate_y(float angle);
    static mat4 rotate_z(float angle);
    static mat4 perspective(float fov,float aspect,float near,float far);
    static mat4 lookAt(const vec3&eye,const vec3&target,const vec3&up);
    mat4& Applytranslate(float x,float y, float z);
    mat4& Applyscale(float x,float y,float z);


};
