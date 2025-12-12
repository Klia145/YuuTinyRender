#pragma once
#include"config.h"
// 检测是否启用现代配置
#ifdef USE_MODERN_CONFIG
    #include "Config_Modern.hpp"
    
    // 映射到全局命名空间（让旧代码无缝使用）
    inline constexpr int width = Config::width;
    inline constexpr int height = Config::height;
    /*
    inline constexpr auto outputCubeRender = Config::outputCubeRender;
    inline constexpr auto output_head = Config::output_head;
    inline constexpr auto output_path = Config::output_path;
    inline constexpr auto African_Head = Config::African_Head;
    */
    inline constexpr auto African_Head_Diffuse = Config::African_Head_Diffuse;
    inline constexpr float FLT_MAX= Config::FLT_MAX;
    
    inline constexpr float MouseSensitivity = Config::MouseSensitivity;
    inline constexpr float ROTATE_SENSITIVITY = Config::ROTATE_SENSITIVITY;
    inline constexpr float PAN_SENSITIVITY = Config::PAN_SENSITIVITY;
    inline constexpr float ZOOM_SENSITIVITY = Config::ZOOM_SENSITIVITY;
    
    inline constexpr auto MSAA_4X_OFFSETS = Config::MSAA_4X_OFFSETS;
    
    inline constexpr float Fov = Config::Camera::Fov;
    inline constexpr float aspect = Config::Camera::aspect;
    inline constexpr float Near = Config::Camera::Near;
    inline constexpr float Far = Config::Camera::Far;
    inline constexpr float CameraDistance=Config::Camera::CameraDistance;
    inline constexpr float WheelZoonSpeed=Config::Camera::WheelZoonSpeed;
    inline constexpr float PI=Config::PI;

    inline constexpr float FogStart=Config::Fog::FogStart;
    inline constexpr float FogEnd=Config::Fog::FogEnd;
    inline constexpr int FogColorR=Config::Fog::FogColorR;
    inline constexpr int FogColorG=Config::Fog::FogColorG;
    inline constexpr int FogColorB=Config::Fog::FogColorB;

    inline constexpr int current_group=Config::ModelManager::current_group;
    inline constexpr int current_model=Config::ModelManager::current_model_index;
    
#else
    // 旧版本（运行时常量，需要.cpp文件定义）
    #include <string>
    
    extern const int width;
    extern const int height;
    extern const std::string outputCubeRender;
    extern const std::string output_head;
    extern const std::string output_path;
    extern const std::string African_Head;
    extern const std::string African_Head_Diffuse;
    extern const float MSAA_4X_OFFSETS[4][2];
    extern const float MouseSensitivity;
    extern const float ROTATE_SENSITIVITY;
    extern const float PAN_SENSITIVITY;
    extern const float ZOOM_SENSITIVITY;
    extern const float Fov;
    extern const float aspect;
    extern const float Near;
    extern const float Far;
    extern const float CameraDistance;
    extern const float PI;
#endif