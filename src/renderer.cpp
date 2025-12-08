#include "renderer.h"
#include "geometry.h"
#include "utils.h"
#include"constants.h"
#include<colorTable.h>
void Line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for(float t = 0; t < 1; t += 0.01) {
        int x = x0 + (x1 - x0) * t;
        int y = y0 + (y1 - y0) * t;
        image.set(x, y, color);
    }
}

void BresenhamLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int x = x0;
    int y = y0;
    int error = 0;

    int x_step = x0 < x1 ? 1 : -1;
    int y_step = y0 < y1 ? 1 : -1;
    
    if(dx > dy) {
        for(int i = 0; i <= dx; i++) {
            image.set(x, y, color);
            x += x_step;
            error += dy;
            if(error >= dx) {
                y += y_step;
                error -= dx;
            }
        }
    } else {
        for(int i = 0; i <= dy; i++) {
            image.set(x, y, color);
            y += y_step;
            error += dx;
            if(error >= dy) {
                x += x_step;
                error -= dy;
            }
        }
    }
}

void Scan_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color) {
    BresenhamLine(x0, y0, x1, y1, image, color);
    BresenhamLine(x1, y1, x2, y2, image, color);
    BresenhamLine(x2, y2, x0, y0, image, color);
    
    if(y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    if(y0 > y2) {
        std::swap(x0, x2);
        std::swap(y0, y2);
    }
    if(y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    int total_height = y2 - y0;
    for(int y = y0; y <= y1; y++) {
        int segment_height = y1 - y0 + 1;
        float alpha = (float)(y - y0) / total_height;
        float beta = (float)(y - y0) / segment_height;

        int xa = x0 + (x2 - x0) * alpha;
        int xb = x0 + (x1 - x0) * beta;
        if(xa > xb) {
            std::swap(xa, xb);
        }
        for(int x = xa; x <= xb; x++) {
            image.set(x, y, color);
        }
    }
    
    for(int y = y1; y <= y2; y++) {
        int segment_height = y2 - y1 + 1;
        float alpha = (float)(y - y0) / total_height;
        float beta = (float)(y - y1) / segment_height;

        int xa = x0 + (x2 - x0) * alpha;
        int xb = x1 + (x2 - x1) * beta;

        if(xa > xb) {
            std::swap(xa, xb);
        }
        for(int x = xa; x <= xb; x++) {
            image.set(x, y, color);
        }
    }
}

void Barycentric_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            int w0 = cross(x1 - x, y1 - y, x2 - x, y2 - y);
            int w1 = cross(x2 - x, y2 - y, x0 - x, y0 - y);
            int w2 = cross(x0 - x, y0 - y, x1 - x, y1 - y);
            
            if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                image.set(x, y, color);
            }
        }
    }
}

void Barycentric_triangle_gradient(int x0, int y0, TGAColor color, int x1, int y1, TGAColor color2, 
                                  int x2, int y2, TGAColor color3, TGAImage& image) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));
    
    int total = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            int w0 = cross(x1 - x, y1 - y, x2 - x, y2 - y);
            int w1 = cross(x2 - x, y2 - y, x0 - x, y0 - y);
            int w2 = cross(x0 - x, y0 - y, x1 - x, y1 - y);
            
            if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                float alpha = (float)w0 / total;
                float beta  = (float)w1 / total;
                float gamma = (float)w2 / total;
                
                float r = color.bgra[2] * alpha + color2.bgra[2] * beta + color3.bgra[2] * gamma;
                float g = color.bgra[1] * alpha + color2.bgra[1] * beta + color3.bgra[1] * gamma;
                float b = color.bgra[0] * alpha + color2.bgra[0] * beta + color3.bgra[0] * gamma;
                
                TGAColor Blendcolor(r, g, b, 255);
                image.set(x, y, Blendcolor);
            }
        }
    }
}

// 深度缓冲三角形绘制实现
void Barycentric_triangle_zbuffer(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, 
                                 TGAImage& image, std::vector<float>& zbuffer, TGAColor color) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));
    
    int total = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
    if(total == 0) return;
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            int w0 = cross(x1 - x, y1 - y, x2 - x, y2 - y);
            int w1 = cross(x2 - x, y2 - y, x0 - x, y0 - y);
            int w2 = cross(x0 - x, y0 - y, x1 - x, y1 - y);
            
            if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                float alpha = (float)w0 / total;
                float beta = (float)w1 / total;
                float gamma = (float)w2 / total;
                
                float z = alpha * z0 + beta * z1 + gamma * z2;
                
                int idx = x + y * image.width();
                if(z > zbuffer[idx]) {
                    zbuffer[idx] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, const vec3& light_dir) {
    mat4 mvp = TransformModel_To_View(45);
    Render(image, model, zBuffer, texture, light_dir, mvp);
}

void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, TGAImage& texture, 
            const vec3& light_dir, const mat4& mvp) {
    for(int i = 0; i < model.nfaces(); i++) {
        vec3 v0 = model.vert(i, 0);
        vec3 v1 = model.vert(i, 1);
        vec3 v2 = model.vert(i, 2);

        vec4 v0_clip = mvp * vec4(v0, 1);
        vec4 v1_clip = mvp * vec4(v1, 1);
        vec4 v2_clip = mvp * vec4(v2, 1);

        vec2 uv0 = model.uv(i, 0);
        vec2 uv1 = model.uv(i, 1);
        vec2 uv2 = model.uv(i, 2);

        vec3 normal = normalize((v2 - v0).cross(v1 - v0));
        float intensity = std::max(0.f, normal.dot(vec3(1, 1, 1)));
        float ambient = 0.5f;
        intensity = intensity * 0.5f + ambient;

        auto [x0, y0, z0] = project(v0_clip);
        auto [x1, y1, z1] = project(v1_clip);
        auto [x2, y2, z2] = project(v2_clip);

        triangle_with_texture(x0, y0, z0, uv0, x1, y1, z1, uv1, x2, y2, z2, uv2,
                            image, zBuffer, texture, intensity);
    }
}
void Barycentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2,
                                      TGAImage& image, std::vector<float>& zbuffer, TGAColor color) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));
    
    int total = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
    if(total == 0) return;
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            int samples_inside = 0;
            float depth_sum = 0;
            
            for(int s = 0; s < 4; s++) {
                float sx = x + MSAA_4X_OFFSETS[s][0];
                float sy = y + MSAA_4X_OFFSETS[s][1];
                
                float w0 = cross(x1 - sx, y1 - sy, x2 - sx, y2 - sy);
                float w1 = cross(x2 - sx, y2 - sy, x0 - sx, y0 - sy);
                float w2 = cross(x0 - sx, y0 - sy, x1 - sx, y1 - sy);
                
                if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    samples_inside++;
                    float alpha = w0 / total;
                    float beta = w1 / total;
                    float gamma = w2 / total;
                    float z = alpha * z0 + beta * z1 + gamma * z2;
                    depth_sum += z;
                }
            }
            
            if(samples_inside > 0) {
                float avg_depth = depth_sum / samples_inside;
                float coverage = (float)samples_inside / 4.0f;
                int idx = x + y * image.width();
                if(avg_depth > zbuffer[idx]) {
                    zbuffer[idx] = avg_depth;
                    image.set(x, y, color * coverage);
                }
            }
        }
    }
}

void Bartcentric_triangle_zbuffer_MSAA(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2,
                                      TGAImage& image, std::vector<float>& zbuffer, TGAColor color, const MSAA& msaa) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));

    int total = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
    if(total == 0) return;
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            int samples_inside = 0;
            float depth_sum = 0;
            
            for(int s = 0; s < msaa.getSampleCount(); s++) {
                auto [offset_x, offset_y] = msaa.getSamplePosition(s);
                float sx = x + offset_x;
                float sy = y + offset_y;

                float w0 = cross(x1 - sx, y1 - sy, x2 - sx, y2 - sy);
                float w1 = cross(x2 - sx, y2 - sy, x0 - sx, y0 - sy);
                float w2 = cross(x0 - sx, y0 - sy, x1 - sx, y1 - sy);

                if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    samples_inside++;
                    float alpha = w0 / total;
                    float beta = w1 / total;
                    float gamma = w2 / total;
                    float z = alpha * z0 + beta * z1 + gamma * z2;
                    depth_sum += z;
                }
            }
            
            if(samples_inside > 0) {
                float avg_depth = depth_sum / samples_inside;
                int idx = x + y * image.width();
                if(avg_depth > zbuffer[idx]) {
                    zbuffer[idx] = avg_depth;
                    if(samples_inside < msaa.getSampleCount()) {
                        float coverage = (float)samples_inside / msaa.getSampleCount();
                        image.set(x, y, color * coverage);
                    } else {
                        image.set(x, y, color);
                    }
                }
            }
        }
    }
}

void triangle_with_texture(int x0, int y0, float z0, vec2 uv0, int x1, int y1, float z1, vec2 uv1, 
                          int x2, int y2, float z2, vec2 uv2, TGAImage& image, std::vector<float>& zbuffer,
                          TGAImage& texture, float intensity) {
    int x_min = std::min(x0, std::min(x1, x2));
    int x_max = std::max(x0, std::max(x1, x2));
    int y_min = std::min(y0, std::min(y1, y2));
    int y_max = std::max(y0, std::max(y1, y2));
    x_min=std::max(0,x_min);
    x_max=std::min(width-1,x_max);
    y_min=std::max(0,y_min);
    y_max=std::min(height-1,y_max);

    int total = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
    if(total == 0) return;
    
    for(int x = x_min; x <= x_max; x++) {
        for(int y = y_min; y <= y_max; y++) {
            if(x<0||x>=width||y<0||y>=height) continue;
            int w0 = cross(x1 - x, y1 - y, x2 - x, y2 - y);
            int w1 = cross(x2 - x, y2 - y, x0 - x, y0 - y);
            int w2 = cross(x0 - x, y0 - y, x1 - x, y1 - y);
            
            if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                float alpha = (float)w0 / total;
                float beta = (float)w1 / total;
                float gamma = (float)w2 / total;
                float z = alpha * z0 + beta * z1 + gamma * z2;
                
                int idx = x + y * image.width();
                if(idx<0||idx >= static_cast<int>(zbuffer.size())){
                    continue;
                }
                if(z > zbuffer[idx]) {
                    zbuffer[idx] = z;
                    float u = alpha * uv0.u + beta * uv1.u + gamma * uv2.u;
                    float v = alpha * uv0.v + beta * uv1.v + gamma * uv2.v;

                    int tex_x = u * texture.width();
                    int tex_y = (1.0f - v) * texture.height();
                    tex_x = std::max(0, std::min(tex_x, texture.width() - 1));
                    tex_y = std::max(0, std::min(tex_y, texture.height() - 1));

                    TGAColor tex_color = texture.get(tex_x, tex_y);
                    TGAColor color = tex_color * intensity;
                    image.set(x, y, color);
                }
            }
        }
    }
}

// MSAA测试和渲染函数
void render_with_msaa(const MSAA& msaa, const std::string& filename) {
    const int width = 800;
    const int height = 800;
    TGAColor black(0, 0, 0, 255);
    std::vector<float> zbuffer(width * height, -1e9);
    TGAImage image(width, height, 3, black);
    Model model(African_Head);

    auto project_local = [](vec3 v) -> std::tuple<int, int, float> {
        int margin = 100;
        int x = (v.x + 1.0) * (width - 2 * margin) / 2.0 + margin;
        int y = (1.0 - v.y) * (height - 2 * margin) / 2.0 + margin;
        return {x, y, v.z};
    };

    vec3 light_dir = normalize(vec3(1, 1, 1));

    for(int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        vec3 v0 = model.vert(face[0]);
        vec3 v1 = model.vert(face[1]);
        vec3 v2 = model.vert(face[2]);

        vec3 normal = normalize((v2 - v0).cross(v1 - v0));
        float intensity = std::max(0.f, normal.dot(light_dir));

        auto [x0, y0, z0] = project_local(v0);
        auto [x1, y1, z1] = project_local(v1);
        auto [x2, y2, z2] = project_local(v2);

        int area = cross(x1 - x0, y1 - y0, x2 - x0, y2 - y0);
        if(area <= 0) continue;

        TGAColor color = ColorTable::getColor(ColorName::WHITE) * intensity;

        Bartcentric_triangle_zbuffer_MSAA(x0, y0, z0, x1, y1, z1, x2, y2, z2, image, zbuffer, color, msaa);
    }
    image.write_png_file(output_path + filename);
}

void testMSAAPatterns() {
    MSAA regular(4, MSAA::REGULAR);
    MSAA rotated(4, MSAA::ROTATED);
    MSAA random(4, MSAA::RANDOM);
    MSAA halton(4, MSAA::HALTON);

    render_with_msaa(regular, "msaa_regular.png");
    render_with_msaa(rotated, "msaa_rotated.png");
    render_with_msaa(random, "msaa_random.png");
    render_with_msaa(halton, "msaa_halton.png");
}

void render_rotation_sequence() {
    Model model(African_Head);
    TGAImage texture;
    texture.read_tga_file(African_Head_Diffuse);
    
    vec3 light_dir = normalize(vec3(1, 1, 1));
    
    for(int frame = 0; frame < 36; frame++) {
        float angle = frame * 10 * 3.14159f / 180.0f;
        mat4 mvp = TransformModel_To_View(angle);
        
        TGAImage image(width, height, 3, TGAColor(0,0,0,255));
        for(int y=0;y<height;y++){
            float t=(float)y/height;
            int r=135+(200-135)*t;
            int g=206+(220-206)*t;
            int b=235+(220-235)*t;

            TGAColor sky_color(r,g,b,255);
            for(int x=0;x<width;x++){
                image.set(x,y,sky_color);
            }
        }
        std::vector<float> zbuffer(width * height, -1e9);

        Render(image, model, zbuffer, texture, light_dir, mvp);
        
        char filename[50];
        std::string output_png_group = "angle_group/";
        sprintf(filename, "rotation_%03d.png", frame);
        image.write_png_file(output_path + output_png_group + std::string(filename));
        
        std::cout << "Rendered frame " << frame << std::endl;
    }
}
void render_skyBox(TGAImage &image){
    for(int y=0;y<height;y++){
            float t=(float)y/height;
            int r=135+(200-135)*t;
            int g=206+(220-206)*t;
            int b=235+(220-235)*t;

            TGAColor sky_color(r,g,b,255);
            for(int x=0;x<width;x++){
                image.set(x,y,sky_color);
            }
    }
}