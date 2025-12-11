#pragma once

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-const-variable"
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-const-variable"
#endif


namespace Config {
    constexpr int width = 800; 
    constexpr int height = 800;

    constexpr const char* outputCubeRender = "cube_render.png";
    constexpr const char* output_head = "head.png";
    constexpr const char* output_path = "output_exe/";
    constexpr const char* African_Head = "models/AfricanHead.obj";
    constexpr const char* African_Head_Diffuse = "obj/african_head/african_head_diffuse.tga";
    constexpr const float FLT_MAX=1e-9;

    constexpr float MSAA_4X_OFFSETS[4][2] = {
        {0.25f, 0.25f}, {0.75f, 0.25f},
        {0.25f, 0.75f}, {0.75f, 0.75f}
    };

    constexpr float MouseSensitivity = 0.005f;
    constexpr float ROTATE_SENSITIVITY = 0.01f;
    constexpr float PAN_SENSITIVITY = 0.001f;
    constexpr float ZOOM_SENSITIVITY = 0.3f;

    namespace Camera {
        constexpr float Fov = 45.0f / 180.0f * 3.14159265f;
        constexpr float aspect = static_cast<float>(width) / static_cast<float>(height);
        constexpr float Near = 0.1f;
        constexpr float Far = 100.0f;
        constexpr float MaxPitch = 1.5f;
        constexpr float WheelZoonSpeed=0.5f;
        constexpr float CameraDistance=3.0f;
    }
    constexpr float PI=3.14159265358979323846f;

    namespace Fog {
        constexpr float FogStart = 5.0f;   
        constexpr float FogEnd = 20.0f;   
        constexpr int FogColorR = 135;    
        constexpr int FogColorG = 206;
        constexpr int FogColorB = 235;
    
}
}

#if defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
