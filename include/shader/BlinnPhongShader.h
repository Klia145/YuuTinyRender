#pragma once
#include"IShader.h"
#include "model.h"
#include "tgaimage.h"
#include "mat4.h"
#include "vec3.h"
#include"Light.h"
class BlinnPhongShader:public IShader{
    private:
        Model* model;
        TGAImage* diffuse_map;
        TGAImage* normal_map;
        TGAImage* specular_map;

        mat4 mvp;
        mat4 model_matrix;


        SunLight&light;
        vec3 camera_pos;

        vec3 ambient;    // 环境光系数
        vec3 diffuse;    // 漫反射系数
        vec3 specular;   // 高光系数
        float shininess; // 高光指数

        vec3 varying_normals[3];
        vec2 varying_uvs[3];
        vec3 varying_positions[3];  // 世界坐标
    public:
    BlinnPhongShader(
        Model* m,
        TGAImage* diffuse,
        const mat4& _mvp,
        SunLight&light,
        const vec3& cam_pos,
        TGAImage* normal = nullptr,
        TGAImage* specular = nullptr
    );
    
    virtual vec4 vertex(int iface, int nthvert) override;
    virtual bool fragment(vec3 bar, TGAColor& color) override;

};