#include "DefaultShader.h"
#include<algorithm>
#include<cmath>


DefaultShader::DefaultShader(
    Model*m ,TGAImage*tex,const mat4&mvp,const SunLight&light,const vec3&cam_pos
    ,bool fog
):model(m),texture(tex),mvp(mvp),light(light),camera_pos(cam_pos),enable_fog(fog),sampler(tex,SamplingMode::NEAREST)
{
    // 构造函数只负责初始化Uniforms
    // Varying数据会在vertex()阶段填充
}
vec4 DefaultShader::vertex(int iface,int nthvert)
{
    /**
     *读取顶点位置
     @note
     model->vert(iface,nthvert)返回第iface个面和第nthvert个顶点
     - iface:面索引(第几个三角形)
     - nthvert:顶点索引(0,1,2)
    */
    vec3 pos=model->vert(iface,nthvert);

    vec2 uv=model->uv(iface,nthvert);

    vec3 normal=model->normal(iface,nthvert);

    /**
     *把数组传入varying数组
     *@details
     *GPU会自动用重心坐标插值这些数据
     *但CPU模拟需要手动储存，然后再fragment()里手动插值
     *@example
     *假设现在处理顶点0(nthverth=0)
     *varying_uv[0]=顶点0的UV
     *法线和uv一次类推
    */
    varying_uv[nthvert]=uv;
    varying_normal[nthvert]=normal;
    varying_worldpos[nthvert]=pos;

    /**
     *@details
     *MVP矩阵变换=Projection x view x Model
     *
     *最后返回的是vec4 是裁剪空间的坐标(x,y,z,w)
     *后续做透视除法(x/w,y/w,z/w)

    */
    return mvp*vec4(pos,1.0);
}

bool DefaultShader::fragment(vec3 bar,TGAColor&color)
{
    /**
     *@details
     *插值后的varying数据
     *bar:重心坐标
     *@example
     *bar.x=重心坐标x
     *bar.y=重心坐标y
     *bar.z=重心坐标z
     */
    vec2 uv=varying_uv[0]*bar.x+
            varying_uv[1]*bar.y+
            varying_uv[2]*bar.z;
    uv.v=1.0f-uv.v;
    vec3 normal=(varying_normal[0]*bar.x+
            varying_normal[1]*bar.y+
            varying_normal[2]*bar.z).normalize();
    
    vec3 worldpos=varying_worldpos[0]*bar.x+
            varying_worldpos[1]*bar.y+
            varying_worldpos[2]*bar.z;
    
    /**
     *@details
     *UV坐标范围是[0,1],需要转换到纹理像素坐标中
     *
     *@example
     *假设纹理大小为1024*1024
     *UV=(0.5,0.5)-->像素坐标=(512,512)
    */

    //纹理采样
    TGAColor tex_color=sampler.sample(uv);

    //基础光照
    float diffuse=std::max(0.0f,normal.dot(light.getDirection()));
    //老生常谈，环境光防止死黑
    float ambient=0.2f;
    float intensity=clamp(diffuse*0.8f+ambient,0.0f,1.0f);

    /**
     * 应用光照到纹理颜色
     * 
     * @details 
     * 把纹理颜色乘以光照强度
     * 
     * @note 
     * TGAColor 的通道顺序是 BGRA
     * - bgra[2] = R（红色）
     * - bgra[1] = G（绿色）
     * - bgra[0] = B（蓝色）
     */

    vec3 base_color(
        tex_color.bgra[2]/255.0f*light.getColor().x,
        tex_color.bgra[1]/255.0f*light.getColor().y,
        tex_color.bgra[0]/255.0f*light.getColor().z
    );

    base_color*=intensity;

    if(enable_fog){
        float distance=(worldpos-camera_pos).length();
        float fog_factor=(distance-FogStart)/(FogEnd-FogStart);
        fog_factor=std::max(0.0f,std::min(1.0f,fog_factor));
        vec3 fog_color(
            FogColorR/255.0f,
            FogColorG/255.0f,
            FogColorB/255.0f
        );
        base_color.x=base_color.x*(1.0f-fog_factor)+fog_color.x*fog_factor;
        base_color.y=base_color.y*(1.0f-fog_factor)+fog_color.y*fog_factor;
        base_color.z=base_color.z*(1.0f-fog_factor)+fog_color.z*fog_factor;
    }

    color.bgra[2]=(unsigned char)(std::min(base_color.x*255.0f,255.0f));
    color.bgra[1] = (unsigned char)(std::min(base_color.y * 255.0f, 255.0f));  // G
    color.bgra[0] = (unsigned char)(std::min(base_color.z * 255.0f, 255.0f));  // B
    color.bgra[3] = 255;  // Alpha（不透明）

    /**
     *返回false表示不丢失这个像素
     *@details
     *用于Alpha测试
     *裁剪（某些像素不应该被渲染

     */
    return false;
}