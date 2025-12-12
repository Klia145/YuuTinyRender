#include "shader/BlinnPhongShader.h"
#include "geometry.h"
#include <algorithm>

BlinnPhongShader::BlinnPhongShader(
    Model* m,
    TGAImage* diffuse,
    const mat4& _mvp,
    SunLight& light,
    const vec3& cam_pos,
    TGAImage* normal,
    TGAImage* specular
) : model(m)
  , diffuse_map(diffuse)
  , normal_map(normal)
  , specular_map(specular)
  , mvp(_mvp)
  , light(light)
  , camera_pos(cam_pos)
  , ambient(0.1f, 0.1f, 0.1f)      // 默认环境光
  , diffuse(0.8f, 0.8f, 0.8f)      // 默认漫反射
  , specular(0.5f, 0.5f, 0.5f)     // 默认高光
  , shininess(32.0f)                // 默认高光指数
{

    model_matrix = mat4::identity();
}

vec4 BlinnPhongShader::vertex(int iface, int nthvert) {

    vec3 pos = model->vert(iface, nthvert);
    vec3 normal = model->normal(iface, nthvert);
    vec2 uv = model->uv(iface, nthvert);
    

    varying_positions[nthvert] = pos;  // 世界坐标
    varying_normals[nthvert] = normal;
    varying_uvs[nthvert] = uv;
    

    return mvp * vec4(pos.x, pos.y, pos.z, 1.0f);
}

bool BlinnPhongShader::fragment(vec3 bar, TGAColor& color) {
    // 插值
    vec3 normal = normalize(
        varying_normals[0] * bar.x +
        varying_normals[1] * bar.y +
        varying_normals[2] * bar.z
    );
    
    vec2 uv = 
        varying_uvs[0] * bar.x +
        varying_uvs[1] * bar.y +
        varying_uvs[2] * bar.z;
    uv.v= 1.0f - uv.v;
    
    vec3 world_pos = 
        varying_positions[0] * bar.x +
        varying_positions[1] * bar.y +
        varying_positions[2] * bar.z;
    
    // 采样纹理 -> 材质基础色
    TGAColor tex_color = diffuse_map->get(
        (int)(uv.x * diffuse_map->width()),
        (int)(uv.y * diffuse_map->height())
    );
    vec3 base_color(
        tex_color.bgra[2] / 255.0f,
        tex_color.bgra[1] / 255.0f,
        tex_color.bgra[0] / 255.0f
    );
    
    // 光源颜色（假设是白光）
    vec3 light_color(1.0f, 1.0f, 1.0f);
    
    // === Blinn-Phong 光照计算 ===
    
    // 1. 环境光 = Ka × 光源 × 材质色
    vec3 ambient_light = vec3(
        ambient.x * light_color.x * base_color.x,
        ambient.y * light_color.y * base_color.y,
        ambient.z * light_color.z * base_color.z
    );
    
    // 2. 漫反射 = Kd × max(N·L, 0) × 光源 × 材质色
    float diff = std::max(normal.dot(light.getDirection()), 0.0f);
    vec3 diffuse_light = vec3(
        diffuse.x * diff * light_color.x * base_color.x,
        diffuse.y * diff * light_color.y * base_color.y,
        diffuse.z * diff * light_color.z * base_color.z
    );
    
    // 3. 高光（Blinn-Phong）= Ks × pow(max(N·H, 0), shininess) × 光源
    vec3 view_dir = normalize(camera_pos - world_pos);
    vec3 halfway = normalize(light.getDirection() + view_dir);
    float spec = std::pow(std::max(normal.dot(halfway), 0.0f), shininess);
    
    float spec_intensity = 1.0f;
    if (specular_map) {
        TGAColor spec_tex = specular_map->get(
            (int)(uv.x * specular_map->width()),
            (int)(uv.y * specular_map->height())
        );
        spec_intensity = spec_tex.bgra[0] / 255.0f;
    }
    
    vec3 specular_light = vec3(
        specular.x * spec * spec_intensity * light_color.x*light.getColor().x,
        specular.y * spec * spec_intensity * light_color.y*light.getColor().y,
        specular.z * spec * spec_intensity * light_color.z*light.getColor().z
    );
    
    vec3 final_color = ambient_light + diffuse_light + specular_light;
    
    final_color.x = std::min(1.0f, std::max(0.0f, final_color.x));
    final_color.y = std::min(1.0f, std::max(0.0f, final_color.y));
    final_color.z = std::min(1.0f, std::max(0.0f, final_color.z));
    
    color = TGAColor(
        (unsigned char)(final_color.z * 255),
        (unsigned char)(final_color.y * 255),
        (unsigned char)(final_color.x * 255),
        255
    );
    
    return false;
}