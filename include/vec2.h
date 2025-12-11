#pragma once
#include<iostream>
struct vec2{
    union {
        struct{float u,v;};
        struct {float x,y;};
    };
    vec2(float u=0,float v=0);
    vec2 operator+(const vec2&other) const;
    vec2 operator-(const vec2& other)const;
    vec2 operator*(float t)const;
    friend std::ostream&operator<<(std::ostream&os,const vec2&v);

    
};