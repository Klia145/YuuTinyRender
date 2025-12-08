#pragma once
#include"mat4.h"
enum class ViewPreset {
    FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, DEFAULT
};
class Camera{
    private:
    vec3 position;
    vec3 target;
    vec3 up;

    float fov;
    float aspect;
    float near_plane;
    float far_plane;

    float yaw;
    float pitch;
    float distance;
    

    


    public:
    Camera(vec3 pos,vec3 target,float fov,float aspect,float near,float far);
    mat4 getViewMatrix()const;
    mat4 getProjectionMatrix()const;
    mat4 getViewProjectionMatrix()const;

    void rotate(float delta_yaw,float delta_pitch);
    void zoom(float delta_distance);
    void pan(float delta_x,float delta_y);
    void focusOn(const vec3&target_pos,float view_distance=3.0f);
    void focusPreset(ViewPreset preset,const vec3&target,float view_distance);

    vec3 getPosition()const{return position;}
    vec3 getTarget()const {return target;}
    float getDistance()const{return distance;}
};
