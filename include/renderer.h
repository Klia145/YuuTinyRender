#pragma once
#include "tgaimage.h"
#include "model.h"
#include "antialiasing.h"
#include "mat4.h"
#include <vector>
#include <string>
#include"camera.h"
#include<cmath>
#include"Ishader.h"

/**
 * @brief 使用Shader渲染模型（新版可编程管线）
 * 
 * @param image 输出图像
 * @param model 3D模型
 * @param zbuffer 深度缓冲
 * @param shader Shader接口指针
 */


void RenderWithShader(TGAImage& image,Model&model,std::vector<float>&zbuffer,IShader* shader);

void rasterize_triangle_with_shader(
    vec3* screen_coords,      // 屏幕坐标[3]
    std::vector<float>& zbuffer,
    TGAImage& image,
    IShader* shader,
    int width, int height
);
/**
 * @brief 光栅化三角形（配合Shader使用）
 */
// ==========================================
// 旧版渲染函数（兼容性保留，逐步废弃）
// ==========================================
void BresenhamLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
/**
 * @brief Bresenham画线算法
 */

void render_skyBox(TGAImage&image);
/**
 * @brief 绘制参考网格
 */
void DrawGrid(TGAImage&image,const mat4&mvp,const vec3&camera_pos,const TGAColor&color);
/**
 * @brief 绘制无限网格
 */
void DrawScreenSpaceGrid(TGAImage&image,const mat4&mvp,const Camera&camera,const TGAColor&color);
void applyGammaCorrection(TGAImage& image,float gamma=2.2f);
/**
 * @brief Gamma校正
 */
void DrawLine3D(TGAImage& image, const mat4& vp, vec3 p1, vec3 p2, int w, int h, TGAColor color);
void DrawInfiniteGrid(TGAImage& image, const mat4& view, const mat4& proj, const vec3& camera_pos);

/**
 * @brief 废弃函数。删除


void render_with_msaa(const MSAA& msaa, const std::string& filename);
void Barycentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color);
void Bartcentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color, const MSAA& msaa);
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, const vec3& light_dir);
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, const vec3& light_dir, const mat4& mvp);
void Line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void Scan_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
void Barycentric_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color);
void Barycentric_triangle_gradient(int x0, int y0, TGAColor color, int x1, int y1, TGAColor color2, int x2, int y2, TGAColor color3, TGAImage& image);
void Barycentric_triangle_zbuffer(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color);
void triangle_with_texture(int x0, int y0, float z0, vec2 uv0, int x1, int y1, float z1, vec2 uv1, int x2, int y2, float z2, vec2 uv2, TGAImage& image, std::vector<float>& zbuffer, TGAImage& texture, float intensity);
void render_rotation_sequence();
void testMSAAPatterns();
void triangle_with_texture(
    float x0, float y0, float z0, const vec2& uv0, const vec3& world0,
    float x1, float y1, float z1, const vec2& uv1, const vec3& world1,
    float x2, float y2, float z2, const vec2& uv2, const vec3& world2,
    TGAImage& image, std::vector<float>& zBuffer, TGAImage& texture,
    float intensity,
    const vec3& camera_pos, bool enable_fog,
    const vec3& fog_color, float fog_start, float fog_end
);
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer,
            TGAImage& texture, const vec3& light_dir, const mat4& mvp,
            const vec3& camera_pos, bool enable_fog = false);

*/