#include "material.h"
#include "tgaimage.h"
#include <iostream>

Material::~Material() {
    delete diffuse_tex;
    delete normal_tex;
    delete specular_tex;
    delete glow_tex;
}
void Material::loadTextures() {
    // 加载漫反射贴图
    if (!diffuse_map.empty()) {
        diffuse_tex = new TGAImage();
        if (!diffuse_tex->read_tga_file(diffuse_map.c_str())) {
            std::cerr << "⚠️ 无法加载漫反射贴图: " << diffuse_map << std::endl;
            delete diffuse_tex;
            diffuse_tex = nullptr;
        } else {
            std::cout << "✓ 加载漫反射: " << diffuse_map << std::endl;
        }
    }
    

    if (!normal_map.empty()) {
        normal_tex = new TGAImage();
        if (!normal_tex->read_tga_file(normal_map.c_str())) {
            delete normal_tex;
            normal_tex = nullptr;
        } else {
            std::cout << "✓ 加载法线: " << normal_map << std::endl;
        }
    }
    

    if (!specular_map.empty()) {
        specular_tex = new TGAImage();
        if (!specular_tex->read_tga_file(specular_map.c_str())) {
            delete specular_tex;
            specular_tex = nullptr;
        } else {
            std::cout << "✓ 加载高光: " << specular_map << std::endl;
        }
    }
    
    // 加载自发光贴图
    if (!glow_map.empty()) {
        glow_tex = new TGAImage();
        if (!glow_tex->read_tga_file(glow_map.c_str())) {
            delete glow_tex;
            glow_tex = nullptr;
        } else {
            std::cout << "✓ 加载自发光: " << glow_map << std::endl;
        }
    }
}