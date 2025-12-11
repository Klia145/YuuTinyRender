#pragma once
#include "Ishader.h"
#include "model.h"
#include "tgaimage.h"
#include "mat4.h"
#include "constants.h"
#include "utils.h"
#include"TextureSampler.h"

/**
 * @brief Defalut shader
 * -Lambert基础光照
 * -纹理采样
 * -线性雾
 * 对应的原来的代码

 * 原来在Render()函数中的逻辑，现在分为两个部分:
 * 1.vertex()
 * 2.fragment()


*/

class DefaultShader:public IShader
{
    private:
    /**
     *@brief 模型指针
     *@b 用来读取顶点数据(解码模型中的uv，法线，顶点) 
    */
    Model* model;
    /**
     *@brief 纹理指针
    */

    TGAImage* texture;



    /**
     *@brief MVP矩阵
     *Model-View-Projection组合矩阵
     *用来把顶点从模型空间转到裁剪空间
     */
     mat4 mvp;

    /**
     *brief 光照方向
     *平行光方向
    */
    vec3 light_dir;

    /**
     *@brief 相机位置
     *各种偏移参数。焦距等等
    */
    vec3 camera_pos;

    /** 
     *@brief是否启用雾
    */
    bool enable_fog;

    /**
     *@brief UV坐标
     *@details
     *储存三角形3个顶点的UV坐标
     *-varying_uv[0]=顶点0的uv
     *-varying_uv[1]=顶点1的uv
     *-varying_uv[2]=顶点2的uv

     *在fragment()中用重心坐标插值

     
    */
    vec2 varying_uv[3];
    /**
     *@brief 法线数组
     *储存三角形三个顶点的法线
    */
    vec3 varying_normal[3];

    vec3 varying_worldpos[3];

    TextureSampler sampler;

    public:

    DefaultShader(Model*m,TGAImage*tex,const mat4& _mvp,const vec3&light,const vec3&cam_pos,bool fog);

    vec4 vertex(int iface,int nthvert) override;

    bool fragment(vec3 bar,TGAColor&color)override;
    
    



};