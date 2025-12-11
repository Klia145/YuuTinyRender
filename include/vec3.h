#pragma once
#include<iostream>
struct vec3{
    float x,y,z;
    vec3():x(0),y(0),z(0){}
    vec3(float x,float y,float z):x(x),y(y),z(z){}
    vec3 operator+(const vec3&v)const;
    vec3 operator-(const vec3&v)const;
    vec3 operator*(float t)const;
    vec3& operator*=(float t);
    vec3& operator/=(float t);
    bool operator==(const vec3& v)const;
    float operator*(const vec3&v)const;
    vec3 cross(const vec3&v)const;
    float length()const;
    vec3 normalize()const;
    float dot(const vec3&v)const;
    friend std::ostream& operator<<(std::ostream&os,const vec3&v);
    

    

    
};