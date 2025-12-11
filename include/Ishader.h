#pragma once
#include "geometry.h"
#include "tgaimage.h"
#include "mat4.h"
#include "model.h"

/**
 * @file IShader.h
 * @brief 可编程渲染管线接口
 * 
 * @details 
 * 这是一个抽象基类，模拟 GPU 的可编程管线（Programmable Pipeline）。
 * 任何具体的渲染效果（Phong、Toon、PBR等）都应该继承这个接口。
 * 
 * === HLSL 对应关系 ===
 * 
 * | C++ (CPU模拟)        | HLSL (GPU)              | 职责                     |
 * |---------------------|-------------------------|--------------------------|
 * | IShader::vertex()   | VS_Main (Vertex Shader) | 顶点变换、准备varying     |
 * | IShader::fragment() | PS_Main (Pixel Shader)  | 计算像素颜色              |
 * | varying_xxx         | struct v2f { ... }      | 顶点→片元的插值数据        |
 * | uniforms_xxx        | cbuffer { ... }         | 全局参数（MVP、光照等）    |
 * | Model::vert()       | StructuredBuffer        | 顶点数据读取              |
 * 
 * === 渲染管线流程 ===
 * 
 * ```
 * 输入顶点 
 *    ↓
 * [顶点着色器] vertex()
 *    ↓ (输出裁剪空间坐标 + varying数据)
 * [透视除法 & 光栅化]  ← CPU/GPU自动完成
 *    ↓ (插值varying，生成片元)
 * [片元着色器] fragment()
 *    ↓ (输出颜色)
 * [深度测试 & 混合]
 *    ↓
 * 输出到framebuffer
 * ```
 */
struct IShader {
    virtual ~IShader() {}
    
    // ==========================================
    // 1. Varying（插值变量）
    // ==========================================
    /**
     * @brief Varying 插值变量寄存器
     * 
     * @details 
     * 在 HLSL 中，这对应于：
     * ```hlsl
     * struct v2f {
     *     float4 position : SV_POSITION;
     *     float2 uv       : TEXCOORD0;
     *     float3 normal   : NORMAL;
     *     float3 worldPos : TEXCOORD1;
     * };
     * ```
     * 
     * === CPU 实现方式 ===
     * 
     * 由于 C++ 无法动态定义结构体，我们用**矩阵模拟寄存器**：
     * 
     * - **列（Column）**：代表三角形的 3 个顶点（v0, v1, v2）
     * - **行（Row）**：代表要传递的数据通道
     * 
     * @example
     * 如果需要传递 UV(2维) + WorldPos(3维)，共5个通道：
     * ```cpp
     * // varying 是一个 5x3 的矩阵
     * // varying[0][0] = v0.uv.x
     * // varying[0][1] = v1.uv.x
     * // varying[0][2] = v2.uv.x
     * // 
     * // varying[1][0] = v0.uv.y
     * // varying[1][1] = v1.uv.y
     * // varying[1][2] = v2.uv.y
     * // 
     * // varying[2][0] = v0.worldPos.x
     * // ... 依此类推
     * 
     * // 片元着色器中，用重心坐标插值：
     * float uv_x = varying[0][0] * bar.x + 
     *              varying[0][1] * bar.y + 
     *              varying[0][2] * bar.z;
     * ```
     * 
     * @note 
     * 具体的 Shader 子类应该定义自己的 varying 数据结构。
     * 比如：
     * ```cpp
     * class PhongShader : public IShader {
     *     vec2 varying_uv[3];      // 3个顶点的UV
     *     vec3 varying_normal[3];  // 3个顶点的法线
     *     vec3 varying_worldpos[3]; // 3个顶点的世界坐标
     * };
     * ```
     */
    
    // ==========================================
    // 2. Uniforms（全局常量）
    // ==========================================
    /**
     * @brief Uniforms 全局参数
     * 
     * @details 
     * 在 HLSL 中，这对应于：
     * ```hlsl
     * cbuffer ConstantBuffer : register(b0) {
     *     float4x4 MVP;
     *     float3 LightDir;
     *     float3 CameraPos;
     *     // ...
     * };
     * ```
     * 
     * === CPU 实现方式 ===
     * 
     * 每个 Shader 子类应该持有需要的 Uniforms：
     * 
     * @example
     * ```cpp
     * class PhongShader : public IShader {
     * private:
     *     // Uniforms（所有像素共享）
     *     mat4 mvp;
     *     vec3 light_dir;
     *     vec3 camera_pos;
     *     TGAImage* texture;
     * 
     * public:
     *     PhongShader(mat4 _mvp, vec3 _light, ...) 
     *         : mvp(_mvp), light_dir(_light), ... {}
     * };
     * ```
     * 
     * @note 
     * Uniforms 在整个三角形渲染过程中保持不变，
     * 而 Varying 是逐顶点/逐像素变化的。
     */
    
    // ==========================================
    // 3. Vertex Shader（顶点着色器）
    // ==========================================
    /**
     * @brief 顶点着色器（处理单个顶点）
     * 
     * @param iface   当前处理的面索引（用于从Model读取数据）
     * @param nthvert 当前处理的是三角形的第几个顶点（0, 1, 或 2）
     * 
     * @return vec4 裁剪空间坐标（Clip Space Coordinates）
     *              对应 HLSL 的 SV_POSITION
     * 
     * @details 
     * === HLSL 对应 ===
     * ```hlsl
     * v2f VS_Main(appdata v) {
     *     v2f o;
     *     
     *     // 1. MVP 变换
     *     o.position = mul(UNITY_MATRIX_MVP, float4(v.vertex, 1.0));
     *     
     *     // 2. 传递 varying 数据
     *     o.uv = v.uv;
     *     o.normal = v.normal;
     *     o.worldPos = mul(unity_ObjectToWorld, v.vertex).xyz;
     *     
     *     return o;
     * }
     * ```
     * 
     * === 职责 ===
     * 1. **读取顶点数据**：从 Model 读取位置、UV、法线等
     * 2. **坐标变换**：模型空间 → 世界空间 → 裁剪空间
     * 3. **准备 Varying**：将需要传递给片元着色器的数据存入 varying 寄存器
     * 
     * @example
     * ```cpp
     * vec4 vertex(int iface, int nthvert) override {
     *     // 1. 读取顶点数据
     *     vec3 pos = model->vert(iface, nthvert);
     *     vec2 uv = model->uv(iface, nthvert);
     *     vec3 normal = model->normal(iface, nthvert);
     *     
     *     // 2. 存入 varying（给 fragment 用）
     *     varying_uv[nthvert] = uv;
     *     varying_normal[nthvert] = normal;
     *     varying_worldpos[nthvert] = pos;
     *     
     *     // 3. MVP 变换
     *     return mvp * vec4(pos, 1.0f);
     * }
     * ```
     * 
     * @note 
     * 返回的裁剪空间坐标会被管线自动进行：
     * - 透视除法（x/w, y/w, z/w）
     * - 视口变换（NDC → 屏幕坐标）
     */
    virtual vec4 vertex(int iface, int nthvert) = 0;
    
    // ==========================================
    // 4. Fragment Shader（片元着色器）
    // ==========================================
    /**
     * @brief 片元着色器（处理单个像素）
     * 
     * @param bar   重心坐标（Barycentric Coordinates）(alpha, beta, gamma)
     *              满足：bar.x + bar.y + bar.z = 1.0
     *              这是 GPU 硬件传给你的插值权重。
     * 
     * @param color [输出] 计算出的像素颜色（RGB，范围 [0, 255]）
     *              对应 HLSL 的 SV_Target
     * 
     * @return bool 
     *         - true：丢弃该像素（对应 HLSL 的 discard）
     *         - false：保留该像素
     * 
     * @details 
     * === HLSL 对应 ===
     * ```hlsl
     * float4 PS_Main(v2f i) : SV_Target {
     *     // 1. 采样纹理
     *     float4 texColor = tex2D(_MainTex, i.uv);
     *     
     *     // 2. 计算光照
     *     float3 N = normalize(i.normal);
     *     float3 L = normalize(_LightDir);
     *     float diffuse = max(0, dot(N, L));
     *     
     *     // 3. 返回最终颜色
     *     return texColor * diffuse;
     * }
     * ```
     * 
     * === 职责 ===
     * 1. **插值 Varying**：用重心坐标混合 3 个顶点的 varying 数据
     *    （GPU 自动做，CPU 需要手动计算）
     * 2. **纹理采样**：从纹理贴图读取颜色
     * 3. **光照计算**：Phong、Blinn-Phong、PBR 等
     * 4. **雾效/后处理**：距离雾、高度雾等
     * 5. **输出颜色**：最终的像素颜色
     * 
     * @example
     * ```cpp
     * bool fragment(vec3 bar, TGAColor &color) override {
     *     // 1. 插值 varying 数据（GPU 自动做，CPU 手动）
     *     vec2 uv = varying_uv[0] * bar.x + 
     *               varying_uv[1] * bar.y + 
     *               varying_uv[2] * bar.z;
     *     
     *     vec3 normal = (varying_normal[0] * bar.x + 
     *                    varying_normal[1] * bar.y + 
     *                    varying_normal[2] * bar.z).normalize();
     *     
     *     // 2. 采样纹理
     *     TGAColor tex = texture->get(uv.x * width, uv.y * height);
     *     
     *     // 3. 计算光照
     *     float diffuse = std::max(0.0f, normal.dot(light_dir));
     *     
     *     // 4. 输出颜色
     *     color.r = tex.r * diffuse;
     *     color.g = tex.g * diffuse;
     *     color.b = tex.b * diffuse;
     *     
     *     return false;  // 不丢弃
     * }
     * ```
     * 
     * @note 
     * 重心坐标的使用：
     * - GPU 硬件自动插值 varying
     * - CPU 模拟时需要手动计算
     * - 插值公式：`result = v0 * bar.x + v1 * bar.y + v2 * bar.z`
     */
    virtual bool fragment(vec3 bar, TGAColor &color) = 0;
};

/**
 * @example 完整的 Shader 实现示例
 * 
 * ```cpp
 * class SimplePhongShader : public IShader {
 * private:
 *     // === Varying（逐顶点数据）===
 *     vec2 varying_uv[3];
 *     vec3 varying_normal[3];
 *     vec3 varying_worldpos[3];
 *     
 *     // === Uniforms（全局参数）===
 *     Model* model;
 *     mat4 mvp;
 *     vec3 light_dir;
 *     vec3 camera_pos;
 *     TGAImage* texture;
 *     
 * public:
 *     SimplePhongShader(Model* m, mat4 _mvp, vec3 light, TGAImage* tex)
 *         : model(m), mvp(_mvp), light_dir(light), texture(tex) {}
 *     
 *     vec4 vertex(int iface, int nthvert) override {
 *         vec3 pos = model->vert(iface, nthvert);
 *         varying_uv[nthvert] = model->uv(iface, nthvert);
 *         varying_normal[nthvert] = model->normal(iface, nthvert);
 *         varying_worldpos[nthvert] = pos;
 *         
 *         return mvp * vec4(pos, 1.0f);
 *     }
 *     
 *     bool fragment(vec3 bar, TGAColor &color) override {
 *         vec2 uv = varying_uv[0] * bar.x + varying_uv[1] * bar.y + varying_uv[2] * bar.z;
 *         vec3 N = (varying_normal[0] * bar.x + varying_normal[1] * bar.y + varying_normal[2] * bar.z).normalize();
 *         
 *         TGAColor tex = texture->get(uv.x * texture->width(), uv.y * texture->height());
 *         float diffuse = std::max(0.0f, N.dot(light_dir));
 *         
 *         color.r = tex.r * diffuse;
 *         color.g = tex.g * diffuse;
 *         color.b = tex.b * diffuse;
 *         
 *         return false;
 *     }
 * };
 * ```
 */