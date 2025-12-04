#include"vec2.h"
vec2::vec2(float x, float y) : u(x), v(y) {}
vec2 vec2::operator+(const vec2& other)const{
    return vec2(u+other.u,v+other.v);
}

vec2 vec2::operator-(const vec2&other)const{
    return vec2(u-other.u,v-other.v);
}
vec2 vec2::operator*(float t)const{
    return vec2(u*t,v*t);
}
std::ostream&operator<<(std::ostream&os,const vec2&v){
    os<<"()"<<v.u<<","<<v.v<<")";
    return os;
}
