#pragma once
#include "tgaimage.h"
#include "model.h"
#include "antialiasing.h"
#include "mat4.h"
#include <vector>
#include <string>

void Line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void BresenhamLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void Scan_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
void Barycentric_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color);
void Barycentric_triangle_gradient(int x0, int y0, TGAColor color, int x1, int y1, TGAColor color2, int x2, int y2, TGAColor color3, TGAImage& image);
void Barycentric_triangle_zbuffer(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color);
void Barycentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color);
void Bartcentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, TGAImage& image, std::vector<float>& zbuffer, TGAColor color, const MSAA& msaa);
void triangle_with_texture(int x0, int y0, float z0, vec2 uv0, int x1, int y1, float z1, vec2 uv1, int x2, int y2, float z2, vec2 uv2, TGAImage& image, std::vector<float>& zbuffer, TGAImage& texture, float intensity);
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, const vec3& light_dir);
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, const vec3& light_dir, const mat4& mvp);
void render_with_msaa(const MSAA& msaa, const std::string& filename);
void testMSAAPatterns();
void render_rotation_sequence();
void render_skyBox(TGAImage&image);