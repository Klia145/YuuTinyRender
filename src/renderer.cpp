#include "renderer.h"
#include "geometry.h"
#include "utils.h"
#include"constants.h"
#include<colorTable.h>
#include <omp.h>

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
void DrawGrid(TGAImage&image,const mat4&mvp,const vec3&camera_pos,const TGAColor&color){

    int width=image.width();
    int height=image.height();

    float camera_height = std::abs(camera_pos.y);
    if (camera_height < 0.1f) camera_height = 1.0f;  
    
    float cell_size = 1.0f;
    if (camera_height > 10.0f) cell_size = 2.0f;
    if (camera_height > 20.0f) cell_size = 5.0f;
    if (camera_height > 50.0f) cell_size = 10.0f;
    
    int grid_count = std::min(20, (int)(camera_height * 2.0f / cell_size));
    if (grid_count < 5) grid_count = 5;  
    
    float half_size = grid_count * cell_size / 2.0f;
    float step = cell_size;  



    for(int i=0;i<=grid_count;i++){
        float z=-half_size+i*step;
        vec3 p1(-half_size,0.0f,z);
        vec3 p2(half_size,0.0f,z);

        vec4 v1=mvp*vec4(p1.x,p1.y,p1.z,1.0f);
        vec4 v2=mvp*vec4(p2.x,p2.y,p2.z,1.0f);
         if (v1.w > 0.0001f && v2.w > 0.0001f) {
            v1.x /= v1.w; v1.y /= v1.w;
            v2.x /= v2.w; v2.y /= v2.w;
            
            // NDC -> 屏幕坐标
            int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
            int y1 = (int)((1.0f - v1.y) * 0.5f * height);
            int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
            int y2 = (int)((1.0f - v2.y) * 0.5f * height);
            
            // 绘制线段
            BresenhamLine(x1, y1, x2, y2, image, color);
        }

    }
    for(int i=0;i<=grid_count;i++){
        float x=-half_size+i*step;
        vec3 p1(x,0.0f,-half_size);
        vec3 p2(x,0.0f,half_size);

        vec4 v1=mvp*vec4(p1.x,p1.y,p1.z,1.0f);
        vec4 v2=mvp*vec4(p2.x,p2.y,p2.z,1.0f);
        if (v1.w > 0.0001f && v2.w > 0.0001f) {
            v1.x /= v1.w; v1.y /= v1.w; 
            v2.x /= v2.w; v2.y /= v2.w;
            
            int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
            int y1 = (int)((1.0f - v1.y) * 0.5f * height);
            int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
            int y2 = (int)((1.0f - v2.y) * 0.5f * height);
            
            BresenhamLine(x1, y1, x2, y2, image, color); 
        }
    }
}
void DrawScreenSpaceGrid(TGAImage&image,const mat4&mvp,const Camera&camera,const TGAColor&color){
    int width=image.width();
    int height=image.height();

    int grid_spacing=50;
    vec3 camera_pos=camera.getPosition();

    for(int screen_x=0;screen_x<width;screen_x+=grid_spacing){
        vec3 ray_top=camera.screenToWorldRay(screen_x,0,width,height);
        vec3 ray_bottom=camera.screenToWorldRay(screen_x,height,width,height);

        vec3 p1 = camera.rayGroundIntersection(camera_pos, ray_top);
        vec3 p2 = camera.rayGroundIntersection(camera_pos, ray_bottom);
        if(p1.length()<1000.0f&&p2.length()<1000.0f){
            vec4 v1 = mvp * vec4(p1.x, p1.y, p1.z, 1.0f);
            vec4 v2 = mvp * vec4(p2.x, p2.y, p2.z, 1.0f);
            if (v1.w > 0.0001f && v2.w > 0.0001f) {
                v1.x /= v1.w; v1.y /= v1.w;
                v2.x /= v2.w; v2.y /= v2.w;
                
                int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
                int y1 = (int)((1.0f - v1.y) * 0.5f * height);
                int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
                int y2 = (int)((1.0f - v2.y) * 0.5f * height);
                
                BresenhamLine(x1, y1, x2, y2, image, color);
            }
        }
        
    }
    for (int screen_y = 0; screen_y < height; screen_y += grid_spacing) {
        vec3 ray_left = camera.screenToWorldRay(0, screen_y, width, height);
        vec3 ray_right = camera.screenToWorldRay(width, screen_y, width, height);
        
        vec3 p1 = camera.rayGroundIntersection(camera_pos, ray_left);
        vec3 p2 = camera.rayGroundIntersection(camera_pos, ray_right);
        
        if (p1.length() < 1000.0f && p2.length() < 1000.0f) {
            vec4 v1 = mvp * vec4(p1.x, p1.y, p1.z, 1.0f);
            vec4 v2 = mvp * vec4(p2.x, p2.y, p2.z, 1.0f);
            
            if (v1.w > 0.0001f && v2.w > 0.0001f) {
                v1.x /= v1.w; v1.y /= v1.w;
                v2.x /= v2.w; v2.y /= v2.w;
                
                int x1 = (int)((v1.x + 1.0f) * 0.5f * width);
                int y1 = (int)((1.0f - v1.y) * 0.5f * height);
                int x2 = (int)((v2.x + 1.0f) * 0.5f * width);
                int y2 = (int)((1.0f - v2.y) * 0.5f * height);
                
                BresenhamLine(x1, y1, x2, y2, image, color);
            }
        }
    }

}
void applyGammaCorrection(TGAImage& image, float gamma) {
    static unsigned char gamma_table[256];
    static bool table_initialized = false;

    if (!table_initialized) {
        const float inv_gamma = 1.0f / gamma;
        for (int i = 0; i < 256; i++) {
            float normalized = i / 255.0f;
            float corrected = std::pow(normalized, inv_gamma);
            gamma_table[i] = (unsigned char)(corrected * 255.0f);
        }
        table_initialized = true;
    }

    int width = image.width();
    int height = image.height();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGAColor color = image.get(x, y);
            
            color.bgra[2] = gamma_table[color.bgra[2]];  // R
            color.bgra[1] = gamma_table[color.bgra[1]];  // G
            color.bgra[0] = gamma_table[color.bgra[0]];  // B
            
            image.set(x, y, color);
        }
    }
}
vec3 barycentric(vec2 A, vec2 B, vec2 C, vec2 P) {
    vec3 s[2];
    s[0] = vec3(C.u - A.u, B.u - A.u, A.u - P.u);
    s[1] = vec3(C.v - A.v, B.v - A.v, A.v - P.v);

    vec3 u = s[0].cross(s[1]);
    if (std::abs(u.z) < 1e-2) return vec3(-1, 1, 1);
    return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    //三角形算叉积
}
void DrawLine3D(TGAImage& image, const mat4& vp, vec3 p1, vec3 p2, int w, int h, TGAColor color) {
    vec4 v1 = vp * vec4(p1.x, p1.y, p1.z, 1.0f);
    vec4 v2 = vp * vec4(p2.x, p2.y, p2.z, 1.0f);

    if (v1.w < 0.1f && v2.w < 0.1f) return;
    if (v1.w < 0.1f) { float t = (0.1f - v1.w)/(v2.w - v1.w); v1 = v1 + (v2 - v1)*t; }
    if (v2.w < 0.1f) { float t = (0.1f - v2.w)/(v1.w - v2.w); v2 = v2 + (v1 - v2)*t; }

    v1.x /= v1.w; v1.y /= v1.w;
    v2.x /= v2.w; v2.y /= v2.w;


    int x1 = (int)((v1.x + 1.0f) * 0.5f * w);
    int y1 = (int)((1.0f - v1.y) * 0.5f * h);
    int x2 = (int)((v2.x + 1.0f) * 0.5f * w);
    int y2 = (int)((1.0f - v2.y) * 0.5f * h);

    if ((x1 < 0 && x2 < 0) || (x1 >= w && x2 >= w) || 
    (y1 < 0 && y2 < 0) || (y1 >= h && y2 >= h)) {
    return; 
    }          


    BresenhamLine( x1, y1, x2, y2, image,color);
}
void DrawInfiniteGrid(TGAImage& image, const mat4& view, const mat4& proj, const vec3& camera_pos) {
    int width = image.width();
    int height = image.height();
    
    float grid_spacing = 2.0f;  
    int grid_radius = 20;      
    TGAColor color_grid(120, 120, 120, 255); 
    TGAColor color_x_axis(255, 50, 50, 255); 
    TGAColor color_z_axis(50, 50, 255, 255); 
    
    mat4 vp = proj * view; 

    float snap_x = std::floor(camera_pos.x / grid_spacing) * grid_spacing;
    float snap_z = std::floor(camera_pos.z / grid_spacing) * grid_spacing;
    #pragma omp parallel for schedule(dynamic)
    for (int i = -grid_radius; i <= grid_radius; i++) {
        // 当前线的世界坐标 X
        float x = snap_x + i * grid_spacing;
        

        vec3 p1(x, 0.0f, snap_z - grid_radius * grid_spacing);
        vec3 p2(x, 0.0f, snap_z + grid_radius * grid_spacing);


        TGAColor current_color = (std::abs(x) < 0.001f) ? color_z_axis : color_grid;

        DrawLine3D(image, vp, p1, p2, width, height, current_color);
    }
    for (int i = -grid_radius; i <= grid_radius; i++) {
        float z = snap_z + i * grid_spacing;
        
        vec3 p1(snap_x - grid_radius * grid_spacing, 0.0f, z);
        vec3 p2(snap_x + grid_radius * grid_spacing, 0.0f, z);

        TGAColor current_color = (std::abs(z) < 0.001f) ? color_x_axis : color_grid;

        DrawLine3D(image, vp, p1, p2, width, height, current_color);
    }
}

void triangle_with_texture(
    float x0, float y0, float z0, const vec2& uv0, const vec3& world0,  
    float x1, float y1, float z1, const vec2& uv1, const vec3& world1,
    float x2, float y2, float z2, const vec2& uv2, const vec3& world2,
    TGAImage& image, std::vector<float>& zBuffer, TGAImage& texture, 
    float intensity,
    const vec3& camera_pos, bool enable_fog, 
    const vec3& fog_color, float fog_start, float fog_end) {
    
    int width = image.width();
    int height = image.height();
    //寻找最远和最近的坐标
    int min_x = std::max(0, (int)std::min(x0, std::min(x1, x2)));
    int min_y = std::max(0, (int)std::min(y0,std::min(y1,y2)));
    int max_x = std::min(width - 1, (int)std::max(x0,std::max(x1,x2)));
    int max_y = std::min(height - 1, (int)std::max(y0,std::max(y1,y2)));
    
    //遍历三角中的每一个像素
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            vec3 bc = barycentric(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2), 
                                 vec2(x + 0.5f, y + 0.5f));
            
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            
            //计算深度通过重心坐标进行远近插值
            float z = z0 * bc.x + z1 * bc.y + z2 * bc.z;
            int idx = x + y * width;
            
            if (zBuffer[idx] < z) {
                zBuffer[idx] = z;
                
                vec3 world_pos = world0 * bc.x + world1 * bc.y + world2 * bc.z;
                
                vec2 uv = uv0 * bc.x + uv1 * bc.y + uv2 * bc.z;

                int tex_x = (int)(uv.u * texture.width());
                int tex_y = (int)(uv.v * texture.height());
                TGAColor tex_color = texture.get(tex_x, tex_y);

                vec3 color(
                    tex_color.bgra[2] / 255.0f * intensity,
                    tex_color.bgra[1] / 255.0f * intensity,
                    tex_color.bgra[0] / 255.0f * intensity
                );
                
                if (enable_fog) {
                    float distance = (world_pos - camera_pos).length();
                    float fog_factor = (distance - fog_start) / (fog_end - fog_start);
                    fog_factor = std::max(0.0f, std::min(1.0f, fog_factor));
                    
                    color.x = color.x * (1.0f - fog_factor) + fog_color.x * fog_factor;
                    color.y = color.y * (1.0f - fog_factor) + fog_color.y * fog_factor;
                    color.z = color.z * (1.0f - fog_factor) + fog_color.z * fog_factor;
                }
                
                TGAColor final_color(
                    (unsigned char)(color.z * 255),
                    (unsigned char)(color.y * 255),
                    (unsigned char)(color.x * 255),
                    255
                );
                
                image.set(x, y, final_color);
            }
        }
    }
}
void Render(TGAImage& image, const Model& model, std::vector<float>& zBuffer, 
            TGAImage& texture, const vec3& light_dir, const mat4& mvp,
            const vec3& camera_pos, bool enable_fog) {

    const float fog_start = Config::Fog::FogStart;
    const float fog_end = Config::Fog::FogEnd;
    const vec3 fog_color(
        Config::Fog::FogColorR / 255.0f,
        Config::Fog::FogColorG / 255.0f,
        Config::Fog::FogColorB / 255.0f
    );
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
        float intensity = std::max(0.f, normal.dot(light_dir)); 
        float ambient = 0.2f;
        intensity = intensity * 0.8f + ambient;
        auto [x0, y0, z0] = project(v0_clip);
        auto [x1, y1, z1] = project(v1_clip);
        auto [x2, y2, z2] = project(v2_clip);
        triangle_with_texture(
            x0, y0, z0, uv0, v0,  
            x1, y1, z1, uv1, v1,
            x2, y2, z2, uv2, v2,
            image, zBuffer, texture, intensity,
            camera_pos, enable_fog, fog_color, fog_start, fog_end 
        );
    }
}