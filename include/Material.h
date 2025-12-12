#pragma once
#include <string>
#include "vec3.h"

class TGAImage;  

struct Material {
    std::string name;
    
    // 贴图路径
    std::string diffuse_map;
    std::string normal_map;
    std::string specular_map;
    std::string glow_map;

    TGAImage* diffuse_tex;
    TGAImage* normal_tex;
    TGAImage* specular_tex;
    TGAImage* glow_tex;
    

    vec3 Ka; 
    vec3 Kd;  
    vec3 Ks;  
    float Ns; 
    
    Material() 
        : diffuse_tex(nullptr)
        , normal_tex(nullptr)
        , specular_tex(nullptr)
        , glow_tex(nullptr)
        , Ka(0.2f, 0.2f, 0.2f)
        , Kd(0.8f, 0.8f, 0.8f)
        , Ks(0.5f, 0.5f, 0.5f)
        , Ns(32.0f)
    {}
    
    ~Material();
    

    void loadTextures();
};