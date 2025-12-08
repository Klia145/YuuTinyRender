#include "camera.h"
#include"utils.h"
#include<cmath>

Camera::Camera(vec3 pos,vec3 target,float fov,float aspect,float near,float far):position(pos),target(target),up(0,1,0),
fov(fov),aspect(aspect),near_plane(near),far_plane(far){

    vec3 offset=position-target;
    distance=offset.length();
    yaw=atan2(offset.x,offset.z);
    pitch=asin(offset.y/distance);
}

mat4 Camera::getViewMatrix()const{
    return mat4::lookAt(position,target,up);
}
mat4 Camera::getProjectionMatrix() const{
    return mat4::perspective(fov,aspect,near_plane,far_plane);
}

mat4 Camera::getViewProjectionMatrix()const{
    return getProjectionMatrix()*getViewMatrix();
}
void Camera::rotate(float delta_yaw,float delta_pitch){
    yaw+=delta_yaw;
    pitch+=delta_pitch;

    const float max_pitch=1.5f;
    if(pitch>max_pitch) pitch=max_pitch;
    if(pitch<-max_pitch)pitch=-max_pitch;

    position.x = target.x + distance * cos(pitch) * sin(yaw);
    position.y = target.y + distance * sin(pitch);
    position.z = target.z + distance * cos(pitch) * cos(yaw);
}
void Camera::zoom(float delta_distance) {
    distance -= delta_distance;
    
    if (distance < 1.5f) distance = 1.5f; 
    if (distance > 30.0f) distance = 30.0f;
    
    position.x = target.x + distance * cos(pitch) * sin(yaw);
    position.y = target.y + distance * sin(pitch);
    position.z = target.z + distance * cos(pitch) * cos(yaw);
}
void Camera::pan(float delta_x, float delta_y) {
    vec3 forward = normalize(target - position);
    vec3 right = normalize(forward.cross(up));
    vec3 camera_up = right.cross(forward);
    
    vec3 offset = right * delta_x + camera_up * delta_y;
    
    target = target + offset;
    position = position + offset;
}
void Camera::focusOn(const vec3&target_pos,float view_distance){
    target=target_pos;
    distance=view_distance;

    position.x=target.x+distance*std::cos(pitch)*std::sin(yaw);
    position.y=target.y+distance*std::sin(pitch);
    position.z=target.z+distance*std::cos(pitch)*std::cos(yaw);

}
void Camera::focusPreset(ViewPreset preset,const vec3&target_pos,float view_distance){
    target = target_pos;
    distance = view_distance;
    const float PI = 3.14159265f;

    switch(preset){
        case ViewPreset::FRONT:
            yaw=0.0f;
            pitch=0.0f;
            break;
        case ViewPreset::BACK:
            yaw = PI;
            pitch = 0.0f;
            break;
        case ViewPreset::LEFT:
            yaw = -PI / 2.0f;
            pitch = 0.0f;
            break;
        case ViewPreset::RIGHT:
            yaw = PI / 2.0f;
            pitch = 0.0f;
            break;
        case ViewPreset::TOP:
            yaw = 0.0f;
            pitch=-PI/2.0f+0.1f;
            break;
        case ViewPreset::BOTTOM:
            yaw = 0.0f;
            pitch = PI / 2.0f - 0.1f;
            break;
        case ViewPreset::DEFAULT:
            yaw = PI / 4.0f;      // 45度
            pitch = PI / 6.0f;    // 30度
            break;


    }
    position.x = target.x + distance * std::cos(pitch) * std::sin(yaw);
    position.y = target.y + distance * std::sin(pitch);
    position.z = target.z + distance * std::cos(pitch) * std::cos(yaw);
}