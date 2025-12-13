#pragma once
#include"mat4.h"
enum class ViewPreset {
    FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, DEFAULT
};
enum class ProjectionMode{
    PERSPECTIVE,ORTHOGRAPHIC

};
enum class CameraMode{
    ORBIT,FREE

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

    ProjectionMode projection_mode;
    float ortho_size;

    CameraMode camera_mode;
    vec3 forward;
    

    


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

    void toggleProjectionMode();
    void setProjectionMode(ProjectionMode mode);
    ProjectionMode getProjectionMode()const {return projection_mode;}

    vec3 getPosition()const{return position;}
    vec3 getTarget()const {return target;}
    float getDistance()const{return distance;}

    vec3 screenToWorldRay(int screen_x,int screen_y,int width,int height)const;
    vec3 rayGroundIntersection(const vec3&ray_origin,const vec3&raydir)const;
    void setCameraMode(CameraMode m) { camera_mode = m; }
    CameraMode getCameraMode() const { return camera_mode; }
    
    void rotateFree(float delta_yaw, float delta_pitch);  // 自由旋转
    void rotateOrbit(float delta_yaw, float delta_pitch); // 轨道旋转
};
