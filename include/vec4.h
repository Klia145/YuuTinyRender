#pragma once
#include"vec3.h"
#include<iostream>
struct vec4{
    float x,y,z,w;
    vec4():x(0),y(0),z(0),w(){}
    vec4(vec3 v,float w):x(v.x),y(v.y),z(v.z),w(w){}
    vec4(float x,float y,float z,float w):x(x),y(y),z(z),w(w){}
    vec4 operator+(const vec4&v)const;
    vec4 operator-(const vec4&v)const;
    vec4 operator*(float t)const;
    float operator*(const vec4&v)const;
    vec4 cross(const vec4&v)const;
    float length()const;
    vec4 normalize()const;
    float dot(const vec4&v)const;
    friend std::ostream& operator<<(std::ostream&os,const vec4&v);
    

    
};